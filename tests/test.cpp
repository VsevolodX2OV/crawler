// Copyright 2021 by Enigma

#include <stdexcept>

#include <gtest/gtest.h>

#include <example.hpp>

TEST(Example, EmptyTest) {
    EXPECT_THROW(example(), std::runtime_error);
}
