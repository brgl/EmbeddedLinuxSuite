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
 * @file    unit_String.cpp
 */

#include "ElsUnit.hpp"

#include <els/String.hpp>
#include <els/StringList.hpp>

#include <string>

ELSUNIT_SIMPLE_TESTCASE(String, buildStringBasic)
{
    std::string exp = "String test: This is a basic string, that contains "
            "some numbers: 543 3.14";
    std::string s = els::misc::str::buildString("String %s: This is %c "
            "basic string, that contains some numbers: %d %1.2f",
            "test", 'a', 543, 3.14f);

    ELSUNIT_EXPECT_STRING_EQ(exp, s);
}

ELSUNIT_SIMPLE_TESTCASE(String, buildStringLong)
{
    std::string exp("LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG"
                    "LONGLONGLONGLONGLONGLONGLONGLONG");

    std::string s = els::misc::str::buildString("%s", exp.c_str());

    ELSUNIT_EXPECT_STRING_EQ(exp, s);
};

ELSUNIT_SIMPLE_TESTCASE(String, splitBasic)
{
    std::string str("=somethin=somethin 123456");
    els::misc::StringList sl = els::misc::str::split(str, "= 4");
    ELSUNIT_ASSERT_EQ(4, sl.size());
    ELSUNIT_EXPECT_STRING_EQ(sl.at(0), "somethin");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(1), "somethin");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(2), "123");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(3), "56");
}

ELSUNIT_SIMPLE_TESTCASE(String, splitEmptyParams)
{
    els::misc::StringList sl1 = els::misc::str::split("", "123");
    els::misc::StringList sl2 = els::misc::str::split("123", "");
    ELSUNIT_EXPECT_EQ(0, sl1.size());
    ELSUNIT_ASSERT_EQ(1, sl2.size());
    ELSUNIT_EXPECT_STRING_EQ("123", sl2.at(0));
}

ELSUNIT_SIMPLE_TESTCASE(String, splitNoDelimInString)
{
    std::string str("somethinsomethin");
    els::misc::StringList sl = els::misc::str::split(str, " \t");
    ELSUNIT_ASSERT_EQ(1, sl.size());
    ELSUNIT_EXPECT_STRING_EQ("somethinsomethin", sl.at(0));
}

ELSUNIT_SIMPLE_TESTCASE(String, splitOnlyDelim)
{
    std::string str("+-_       ");
    els::misc::StringList sl = els::misc::str::split(str, "+-_ ");
    ELSUNIT_EXPECT_TRUE(sl.empty());
}

ELSUNIT_SIMPLE_TESTCASE(String, joinBasic)
{
    els::misc::StringList sl;
    sl.push_back("one");
    sl.push_back("two");
    sl.push_back("three");
    ELSUNIT_EXPECT_STRING_EQ("onetwothree", els::misc::str::join(sl));
}

ELSUNIT_SIMPLE_TESTCASE(String, joinEmpty)
{
    els::misc::StringList sl;
    ELSUNIT_EXPECT_TRUE(els::misc::str::join(sl).empty());
}

ELSUNIT_SIMPLE_TESTCASE(String, joinSeparators)
{
    els::misc::StringList sl;
    sl.push_back("one");
    sl.push_back("two");
    sl.push_back("three");
    ELSUNIT_EXPECT_STRING_EQ("one->two->three",
            els::misc::str::join(sl, "->"));
}

ELSUNIT_SIMPLE_TESTCASE(String, isBlank)
{
    ELSUNIT_EXPECT_TRUE(els::misc::str::isBlank("  \t "));
    ELSUNIT_EXPECT_TRUE(els::misc::str::isBlank(std::string()));
    ELSUNIT_EXPECT_FALSE(els::misc::str::isBlank(" abc "));
}

ELSUNIT_SIMPLE_TESTCASE(String, crop)
{
    std::string s1("   something ");
    std::string s2("something");
    std::string s3("    ");
    std::string s4;
    std::string s5(" 1");

    ELSUNIT_EXPECT_STRING_EQ("something ", els::misc::str::crop(s1));
    ELSUNIT_EXPECT_STRING_EQ("something", els::misc::str::crop(s2));
    ELSUNIT_EXPECT_TRUE(els::misc::str::crop(s3).empty());
    ELSUNIT_EXPECT_TRUE(els::misc::str::crop(s4).empty());
    ELSUNIT_EXPECT_STRING_EQ("1", els::misc::str::crop(s5));
}

ELSUNIT_SIMPLE_TESTCASE(String, trim)
{
    std::string s1(" something   ");
    std::string s2("something");
    std::string s3("    ");
    std::string s4;
    std::string s5("1 ");

    ELSUNIT_EXPECT_STRING_EQ(" something", els::misc::str::trim(s1));
    ELSUNIT_EXPECT_STRING_EQ("something", els::misc::str::trim(s2));
    ELSUNIT_EXPECT_TRUE(els::misc::str::trim(s3).empty());
    ELSUNIT_EXPECT_TRUE(els::misc::str::trim(s4).empty());
    ELSUNIT_EXPECT_STRING_EQ("1", els::misc::str::trim(s5));
}

ELSUNIT_SIMPLE_TESTCASE(String, strip)
{
    std::string s1("  something   ");
    std::string s2("something");
    std::string s3("    ");
    std::string s4;
    std::string s5(" 1 ");

    ELSUNIT_EXPECT_STRING_EQ("something", els::misc::str::strip(s1));
    ELSUNIT_EXPECT_STRING_EQ("something", els::misc::str::strip(s2));
    ELSUNIT_EXPECT_TRUE(els::misc::str::strip(s3).empty());
    ELSUNIT_EXPECT_TRUE(els::misc::str::strip(s4).empty());
    ELSUNIT_EXPECT_STRING_EQ("1", els::misc::str::strip(s5));
}


