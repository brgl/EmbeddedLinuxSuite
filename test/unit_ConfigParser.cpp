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
 * @file    unit_ConfigParser.cpp
 */

#include "ElsUnit.hpp"

#include <els/ConfigParser.hpp>
#include <els/Exception.hpp>

namespace {

const std::string testConfigGood(
        "# this is a comment                                \n"
        "                                                   \n"
        "global1= someval1                                  \n"
        "global2 = someval2                                 \n"
        "                                                   \n"
        "[type1:section1]                                   \n"
        "local1 = locval1                                   \n"
        "local2 = locval2                                   \n"
        "[type1 : section2]                                 \n"
        "local1 = locval1                                   \n"
        "local2 = locval2                                   \n"
        "[type2:section1]                                   \n"
        "local1 = \"locval1 \"                              \n"
        "local2 = locval2                                   \n");

const std::string testConfigBad(
        "# this is a comment                                \n"
        "                                                   \n"
        "global1= someval1                                  \n"
        "global2 = someval2                                 \n"
        "                                                   \n"
        "[type1:section1]                                   \n"
        "local1 = locval1                                   \n"
        "local2 = locval2                                   \n"
        "[type1 : section2]                                 \n"
        "local1 = locval1                                   \n"
        "33                                                 \n"
        "local2 = locval2                                   \n"
        "[type2:section1]                                   \n"
        "local1 = \"locval1 \"                              \n"
        "local2 = locval2                                   \n");

}

ELSUNIT_SIMPLE_TESTCASE(ConfigParser, parseStrGood)
{
    els::misc::ConfigParser cp;

    ELSUNIT_EXPECT_NO_THROW(cp.parseStr(testConfigGood));
}

ELSUNIT_SIMPLE_TESTCASE(ConfigParser, parseStrBad)
{
    els::misc::ConfigParser cp;

    ELSUNIT_EXPECT_EXCEPTION(cp.parseStr(testConfigBad),
            els::misc::ConfigParser::SyntaxError);
}


