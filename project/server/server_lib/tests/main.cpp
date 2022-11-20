#include <gtest/gtest.h>

#include "db_header.h"

TEST(FIRST_TEST, NOTHING_REALLY) {
    std::cout << "Server tests" << std::endl;
    EXPECT_EQ(example(), 0);
}