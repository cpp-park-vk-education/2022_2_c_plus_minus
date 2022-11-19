#include <gtest/gtest.h>
#include <iostream>

TEST(FIRST_TEST, NOTHING_REALLY) {
    std::cout << "Global test" << std::endl;
    EXPECT_EQ(false , 0);
}
