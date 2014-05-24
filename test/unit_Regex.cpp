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
 * @file    unit_Regex.cpp
 */

#include "ElsUnit.hpp"

#include <els/Regex.hpp>
#include <els/StringList.hpp>

#include <string>

ELSUNIT_SIMPLE_TESTCASE(Regex, compile)
{
    static const std::string goodp("[a-b][A-B]+");
    static const std::string badp("[(-");

    els::misc::Regex reg;
    ELSUNIT_EXPECT_NO_THROW(reg.compile(goodp));
    ELSUNIT_EXPECT_EXCEPTION(reg.compile(badp), els::misc::Regex::RegexError);
}

ELSUNIT_SIMPLE_TESTCASE(Regex, compileConstr)
{
    static const std::string goodp("[a-b][A-B]+");
    static const std::string badp("[(-");

    ELSUNIT_EXPECT_NO_THROW(els::misc::Regex reg(goodp));
    ELSUNIT_EXPECT_EXCEPTION(els::misc::Regex reg(badp),
            els::misc::Regex::RegexError);
}

ELSUNIT_SIMPLE_TESTCASE(Regex, match)
{
    els::misc::Regex reg("[a-z][0-9][^&][A-Z]+");

    ELSUNIT_EXPECT_NO_THROW(ELSUNIT_EXPECT_TRUE(reg.match("c4%E")));
    ELSUNIT_EXPECT_NO_THROW(ELSUNIT_EXPECT_FALSE(reg.match("a3&Z")));
}

ELSUNIT_SIMPLE_TESTCASE(Regex, notCompiled)
{
    els::misc::Regex reg;

    ELSUNIT_EXPECT_EXCEPTION(reg.match("abc"), els::except::LogicError);
    ELSUNIT_EXPECT_EXCEPTION(reg.findMatches("abc"), els::except::LogicError);
}

ELSUNIT_SIMPLE_TESTCASE(Regex, findMatches)
{
    els::misc::Regex reg("[3-7]+|abc+|[m-o]+4");
    std::string str("ddds44445abcabcccttfdssmmmom44e");
    els::misc::StringList sl;

    ELSUNIT_EXPECT_NO_THROW(sl = reg.findMatches(str));
    ELSUNIT_EXPECT_EQ(5, sl.size());
    ELSUNIT_EXPECT_STRING_EQ(sl.at(0), "44445");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(1), "abc");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(2), "abccc");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(3), "mmmom4");
    ELSUNIT_EXPECT_STRING_EQ(sl.at(4), "4");
}

ELSUNIT_SIMPLE_TESTCASE(Regex, findMatchesNonFound)
{
    els::misc::Regex reg("abc");
    els::misc::StringList sl;

    ELSUNIT_EXPECT_NO_THROW(sl = reg.findMatches("def"));
    ELSUNIT_EXPECT_EQ(0, sl.size());
}

ELSUNIT_SIMPLE_TESTCASE(Regex, reset)
{
    els::misc::Regex reg("abc");

    ELSUNIT_EXPECT_NO_THROW(ELSUNIT_EXPECT_TRUE(reg.match("abc")));
    reg.reset();
    ELSUNIT_EXPECT_EXCEPTION(reg.match("abc"), els::except::LogicError);
}

ELSUNIT_SIMPLE_TESTCASE(Regex, setGetPattern)
{
    els::misc::Regex reg("abc");

    ELSUNIT_EXPECT_STRING_EQ("abc", reg.getPattern());
    reg.setPattern("bca");
    ELSUNIT_EXPECT_NO_THROW(reg.compile());
    ELSUNIT_EXPECT_STRING_EQ("bca", reg.getPattern());
    ELSUNIT_EXPECT_TRUE(reg.match("bca"));
}

