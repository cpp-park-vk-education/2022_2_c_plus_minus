#include <gtest/gtest.h>

#include "Handlers.hpp"

TEST(FIRST_TEST, NOTHING_REALLY) {
    std::cout << "Server tests" << std::endl;
    EXPECT_EQ(example(), 0);
}