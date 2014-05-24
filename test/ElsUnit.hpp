/*
 * Copyright (C) 2013 Bartosz Golaszewski
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/**
 * @file    ElsUnit.hpp
 */

#pragma once

#include <gtest/gtest.h>

inline int runElsUnitTests(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return testing::UnitTest::GetInstance()->Run();
}

#define ELSUNIT_SIMPLE_TESTCASE(TESTNAME, CASENAME)                         \
    TEST(TESTNAME, CASENAME)

#define ELSUNIT_EXPECT_TRUE(CONDITION) EXPECT_TRUE(CONDITION)
#define ELSUNIT_EXPECT_FALSE(CONDITION) EXPECT_FALSE(CONDITION)
#define ELSUNIT_ASSERT_TRUE(CONDITION) ASSERT_TRUE(CONDITION)
#define ELSUNIT_ASSERT_FALSE(CONDITION) ASSERT_FALSE(CONDITION)

#define ELSUNIT_EXPECT_EQ(EXPECTED, ACTUAL) EXPECT_EQ(EXPECTED, ACTUAL)
#define ELSUNIT_EXPECT_NOT_EQ(EXPECTED, ACTUAL) EXPECT_NE(EXPECTED, ACTUAL)
#define ELSUNIT_ASSERT_EQ(EXPECTED, ACTUAL) ASSERT_EQ(EXPECTED, ACTUAL)

#define ELSUNIT_EXPECT_CSTRING_EQ(EXPECTED, ACTUAL)                         \
    EXPECT_STREQ(EXPECTED, ACTUAL)

#define ELSUNIT_EXPECT_STRING_EQ(EXPECTED, ACTUAL)                          \
    EXPECT_EQ(EXPECTED, ACTUAL)

#define ELSUNIT_EXPECT_EXCEPTION(STATEMENT, EXCEPTION)                      \
    EXPECT_THROW(STATEMENT, EXCEPTION)
#define ELSUNIT_EXPECT_NO_THROW(STATEMENT) EXPECT_NO_THROW(STATEMENT)
#define ELSUNIT_ASSERT_NO_THROW(STATEMENT) ASSERT_NO_THROW(STATEMENT)

