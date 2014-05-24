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
 * @file    unit_FilePath.cpp
 */

#include "ElsUnit.hpp"

#include <els/FilePath.hpp>
#include <els/Exception.hpp>

ELSUNIT_SIMPLE_TESTCASE(FilePath, basic)
{
    els::fs::FilePath p("/usr/lib");
    ELSUNIT_EXPECT_STRING_EQ("lib", p.basename());
    ELSUNIT_EXPECT_STRING_EQ("/usr", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, slashEnding)
{
    els::fs::FilePath p("/usr/");
    ELSUNIT_EXPECT_STRING_EQ("usr", p.basename());
    ELSUNIT_EXPECT_STRING_EQ("/", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, noDir)
{
    els::fs::FilePath p("usr");
    ELSUNIT_EXPECT_STRING_EQ("usr", p.basename());
    ELSUNIT_EXPECT_STRING_EQ(".", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, noSlashAtStart)
{
    els::fs::FilePath p("usr/lib");
    ELSUNIT_EXPECT_STRING_EQ("lib", p.basename());
    ELSUNIT_EXPECT_STRING_EQ("usr", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, rootDir)
{
    els::fs::FilePath p("/");
    ELSUNIT_EXPECT_STRING_EQ("/", p.basename());
    ELSUNIT_EXPECT_STRING_EQ("/", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, currentDir)
{
    els::fs::FilePath p(".");
    ELSUNIT_EXPECT_STRING_EQ(".", p.basename());
    ELSUNIT_EXPECT_STRING_EQ(".", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, parentDir)
{
    els::fs::FilePath p("..");
    ELSUNIT_EXPECT_STRING_EQ("..", p.basename());
    ELSUNIT_EXPECT_STRING_EQ(".", p.dirname());
}

ELSUNIT_SIMPLE_TESTCASE(FilePath, nameTooLong)
{
    ELSUNIT_EXPECT_EXCEPTION(els::fs::FilePath p(
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
            ), els::except::InvalidArgument);
}

