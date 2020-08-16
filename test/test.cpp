#include "gtest/gtest.h"
#include "helloworld/helloworld.hpp"

namespace hello::test
{
// Tests that Foo does Xyz.
TEST(hello, should_pass)
{
    EXPECT_EQ(1, 1);
}

TEST(hello, hello_func_ret)
{
    EXPECT_EQ(hello(), "hello world!");
}

}  // namespace hello::test

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}