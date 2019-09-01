/* Author: lipixun
 * Created Time : 2019-08-31 19:49:05
 *
 * File Name: context.h
 * Description:
 *
 *  Context class
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPCOMMON_CPPCOMMON_CONTEXT_H_
#define GITHUB_BYTESPIRIT_CPPCOMMON_CPPCOMMON_CONTEXT_H_

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "status.h"

namespace bytespirit {

class Context final {
 public:
  typedef std::function<void(const Status &)> CancelFunc;

  ~Context() { Cancel(Status()); }

  // Return the status if this context is done
  auto Status() const noexcept -> const Status & { return status_; }
  // Return if this context is done
  auto IsDone() const noexcept -> bool { return done_; }

  // Wait until the context is done
  auto Wait() -> void {
    std::unique_lock<std::mutex> lk(m_);
    cv_.wait(lk, [this] { return done_; });
  }

  // Wait until the context is done or timeout
  auto Wait(std::chrono::seconds timeout) -> bool {
    std::unique_lock<std::mutex> lk(m_);
    if (cv_.wait_for(lk, timeout, [this] { return done_; })) {
      return true;
    }
    return false;
  }

  // Wait until the context is done or deadline
  auto Wait(std::chrono::system_clock::time_point deadline) -> bool {
    std::unique_lock<std::mutex> lk(m_);
    if (cv_.wait_until(lk, deadline, [this] { return done_; })) {
      return true;
    }
    return false;
  }

  // Create a new child context
  auto NewChildContext() -> std::shared_ptr<Context> {
    std::unique_lock<std::mutex> lh(m_);
    if (done_) {
      // This context is already done
      return std::shared_ptr<Context>(new Context(true, status_));
    }
    auto child = std::shared_ptr<Context>(new Context);
    children_.emplace_back(child);
    if (++new_child_count_after_last_clean_ >= 100) {
      CleanChildren();
      new_child_count_after_last_clean_ = 0;
    }
    return child;
  }

  // Create a new child context with a cancel function
  auto NewChildContextWithCancel(CancelFunc *cancel_func) -> std::shared_ptr<Context> {
    std::unique_lock<std::mutex> lh(m_);
    if (done_) {
      // This context is already done
      auto child = std::shared_ptr<Context>(new Context(true, status_));
      if (cancel_func != nullptr) {
        *cancel_func = [child](const ::bytespirit::Status &status) { child->Cancel(status); };
      }
      return child;
    }
    auto child = std::shared_ptr<Context>(new Context);
    if (cancel_func != nullptr) {
      *cancel_func = [child](const ::bytespirit::Status &status) { child->Cancel(status); };
    }
    children_.emplace_back(child);
    if (++new_child_count_after_last_clean_ >= 100) {
      CleanChildren();
      new_child_count_after_last_clean_ = 0;
    }
    return child;
  }

  // Compress the context. Clean useless children
  auto Compress() -> void {
    std::unique_lock<std::mutex> lh(m_);
    CleanChildren();
  }

  // Create a new context (without parent)
  static auto New() -> std::shared_ptr<Context> { return std::shared_ptr<Context>(new Context); }

  // Create a new context (without parent) with a cancel function
  static auto NewWithCancel(CancelFunc *cancel_func) -> std::shared_ptr<Context> {
    auto context = std::shared_ptr<Context>(new Context);
    if (cancel_func != nullptr) {
      *cancel_func = [context](const ::bytespirit::Status &status) { context->Cancel(status); };
    }
    return context;
  }

 private:
  Context() : done_(false), new_child_count_after_last_clean_(0) {}
  Context(bool done, const ::bytespirit::Status &status)
      : done_(done), status_(status), new_child_count_after_last_clean_(0) {}

  auto Cancel(const ::bytespirit::Status &status) -> void {
    if (!done_) {
      std::unique_lock<std::mutex> lh(m_);
      if (!done_) {
        done_ = true;
        status_ = status;
        // Cancel all child context
        for (const auto &weak_child : children_) {
          if (auto child = weak_child.lock()) {
            child->Cancel(status);
          }
        }
        // Clear and release memory
        children_.clear();
        children_.shrink_to_fit();
      }
    }
  }

  auto CleanChildren() -> void {
    std::vector<std::weak_ptr<Context>> compressed_children(children_.size() / 2);
    for (const auto &weak_child : children_) {
      if (!weak_child.expired()) {
        compressed_children.push_back(std::move(weak_child));
      }
    }
    children_.swap(compressed_children);
  }

  bool done_;
  ::bytespirit::Status status_;
  std::mutex m_;
  std::condition_variable cv_;
  std::vector<std::weak_ptr<Context>> children_;
  int new_child_count_after_last_clean_;
};  // namespace bytespirit

}  // namespace bytespirit

#endif  // GITHUB_BYTESPIRIT_CPPCOMMON_CPPCOMMON_CONTEXT_H_
