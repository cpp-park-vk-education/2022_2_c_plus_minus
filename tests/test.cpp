#include <gtest/gtest.h>
#include <iostream>

TEST(FIRST_TEST, NOTHING_REALLY) {
    std::cout << "Global test" << std::endl;
    EXPECT_EQ(0 , 0);
}
int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}