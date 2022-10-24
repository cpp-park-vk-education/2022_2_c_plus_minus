#include <gtest/gtest.h>
#include <iostream>
#include "header.h"

TEST(FIRST_TEST, NOTHING_REALLY) {
    std::cout << "Client tests" << std::endl;
    EXPECT_EQ(example(), 0);
}