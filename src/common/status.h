/* Author: lipixun
 * Created Time : 2018-10-28 15:37:33
 *
 * File Name: status.h
 * Description:
 *
 *  The common return status
 *
 */

#ifndef GITHUB_BYTESPIRIT_CPPCOMMON_CPPCOMMON_STATUS_H_
#define GITHUB_BYTESPIRIT_CPPCOMMON_CPPCOMMON_STATUS_H_

#include <exception>
#include <sstream>
#include <string>

#include "idl/common/status.pb.h"

#define FStatus(ok, code, ...) \
  ::bytespirit::Status::NewWithStringStream(ok, code, ##__VA_ARGS__, "@", __PRETTY_FUNCTION__)
#define FFStatus(ok, code, ...)                                                                                    \
  ::bytespirit::Status::NewWithStringStream(ok, code, ##__VA_ARGS__, "@", __PRETTY_FUNCTION__, "#", __FILE__, ":", \
                                            __LINE__)

namespace bytespirit {

class Status final : public std::exception {
 public:
  explicit Status() : Status(true, 0, "") {}
  explicit Status(bool ok) : Status(ok, 0, "") {}
  explicit Status(bool ok, int32_t code) : Status(ok, code, "") {}
  explicit Status(bool ok, const std::string& message) : Status(ok, 0, message) {}
  explicit Status(bool ok, std::string&& message) : Status(ok, 0, std::forward<std::string>(message)) {}
  explicit Status(bool ok, int32_t code, std::string&& message)
      : ok_(ok), code_(code), message_(std::forward<std::string>(message)) {}
  explicit Status(bool ok, int32_t code, const std::string& message) : ok_(ok), code_(code), message_(message) {}
  ~Status() = default;

  auto ok() const noexcept -> bool { return ok_; }
  auto code() const noexcept -> int32_t { return code_; }
  auto message() const noexcept -> const std::string& { return message_; }
  auto what() const noexcept -> const char* override { return message_.c_str(); }

  // Serialize the object to protobuf object
  auto SerializeToProto() const -> idl::Status {
    idl::Status s;
    s.set_ok(ok_);
    s.set_code(code_);
    s.set_message(message_);
    return s;
  }

  // Parse from protobuf object
  static auto ParseFromProto(const idl::Status s) -> Status { return Status(s.ok(), s.code(), s.message()); }

  // Create the status object by formatted string
  template <typename... Args>
  static auto NewWithStringStream(bool ok, int32_t code, Args&&... args) -> Status {
    std::ostringstream ss;
    (ss << ... << args);
    return Status(ok, code, ss.str());
  }

 private:
  bool ok_;
  int32_t code_;
  std::string message_;
};

}  // namespace bytespirit

#endif  // GITHUB_BYTESPIRIT_CPPCOMMON_CPPCOMMON_STATUS_H_
