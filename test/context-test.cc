/* Author: lipixun
 * Created Time : 2019-08-31 21:21:24
 *
 * File Name: context-test.cc
 * Description:
 *
 *  Text context
 *
 */

#include "gtest/gtest.h"

#include <vector>

#include "common/context.h"

TEST(ContextTest, Basic) {
  bytespirit::Context::CancelFunc cancel_func;
  auto context = bytespirit::Context::NewWithCancel(&cancel_func);
  EXPECT_TRUE(!context->IsDone());
  cancel_func(bytespirit::Status(false));
  EXPECT_TRUE(context->IsDone());
  EXPECT_TRUE(!context->Status().ok());
}

TEST(ContextTest, ChildrenBasic) {
  bytespirit::Context::CancelFunc cancel_func;
  auto context = bytespirit::Context::NewWithCancel(&cancel_func);

  std::vector<std::shared_ptr<bytespirit::Context>> children;
  for (size_t i = 0; i < 10; ++i) {
    children.emplace_back(context->NewChildContext());
  }

  cancel_func(bytespirit::Status(false));
  EXPECT_TRUE(context->IsDone());
  EXPECT_TRUE(!context->Status().ok());
  for (const auto& child : children) {
    EXPECT_TRUE(child->IsDone());
    EXPECT_TRUE(!child->Status().ok());
  }
}

TEST(ContextTest, ChildrenSomeReleased) {
  bytespirit::Context::CancelFunc cancel_func;
  auto context = bytespirit::Context::NewWithCancel(&cancel_func);

  std::vector<std::shared_ptr<bytespirit::Context>> children;
  for (size_t i = 0; i < 10; ++i) {
    children.emplace_back(context->NewChildContext());
  }
  children.resize(1);
  children.shrink_to_fit();

  cancel_func(bytespirit::Status(false));
  EXPECT_TRUE(context->IsDone());
  EXPECT_TRUE(!context->Status().ok());
  for (const auto& child : children) {
    EXPECT_TRUE(child->IsDone());
    EXPECT_TRUE(!child->Status().ok());
  }
}
