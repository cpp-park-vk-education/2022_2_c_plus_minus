#include <gtest/gtest.h>

TEST(FIRST_TEST, NOTHING_REALLY) {
    std::cout << "Server tests" << std::endl;
        EXPECT_EQ(0, 0);
}

int main(){
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}