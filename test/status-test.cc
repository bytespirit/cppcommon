/* Author: lipixun
 * Created Time : 2018-10-28 16:58:15
 *
 * File Name: status-test.cc
 * Description:
 *
 *  The status test
 *
 */

#include "gtest/gtest.h"

#include "cppcommon/status.h"

TEST(StatusTest, Basic) {
  bytespirit::Status status1;
  EXPECT_TRUE(status1.ok());
  EXPECT_TRUE(status1.code() == 0);
  EXPECT_TRUE(status1.message() == "");
  bytespirit::Status status2(false, 1);
  EXPECT_FALSE(status2.ok());
  EXPECT_TRUE(status2.code() == 1);
  EXPECT_TRUE(status2.message() == "");
  bytespirit::Status status3(false, "message_test1");
  EXPECT_FALSE(status3.ok());
  EXPECT_TRUE(status3.code() == 0);
  EXPECT_TRUE(status3.message() == "message_test1");
  bytespirit::Status status4(false, 10, "message_test2");
  EXPECT_FALSE(status4.ok());
  EXPECT_TRUE(status4.code() == 10);
  EXPECT_TRUE(status4.message() == "message_test2");
}

TEST(StatusTest, Log) {
  auto status1 = LogStatus(false, 100, "this", "is", 1, "test");
  EXPECT_FALSE(status1.ok());
  EXPECT_TRUE(status1.code() == 100);
  EXPECT_TRUE(status1.message() == "thisis1test@virtual void StatusTest_Log_Test::TestBody()") << "Actual message:" << status1.message();
}
