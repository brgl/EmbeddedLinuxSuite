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
 * @file    unit_ByteArray.cpp
 */

#include "ElsUnit.hpp"

#include <els/ByteArray.hpp>
#include <els/Exception.hpp>

#include <cstring>
#include <string>

ELSUNIT_SIMPLE_TESTCASE(ByteArray, defaultConstr)
{
    els::misc::ByteArray ba;
    ELSUNIT_EXPECT_EQ(0, ba.get());
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, sizeConstr)
{
    els::misc::ByteArray ba(16);
    ELSUNIT_EXPECT_EQ(16, ba.size());
    ELSUNIT_EXPECT_EQ(0, ::memcmp(static_cast<char*>(ba.get()),
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 16));
    ELSUNIT_EXPECT_EXCEPTION(els::misc::ByteArray(0),
            els::except::InvalidArgument);
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, sizeSrcConstr)
{
    static const char* const str = "ELSELSELSELS";
    els::misc::ByteArray ba(str, sizeof(str));
    ELSUNIT_EXPECT_EQ(sizeof(str), ba.size());
    ELSUNIT_EXPECT_EQ(0, memcmp(str, ba.get(), sizeof(str)));
    ELSUNIT_EXPECT_EXCEPTION(els::misc::ByteArray(0, 0),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_EXCEPTION(els::misc::ByteArray(str, 0),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_EXCEPTION(els::misc::ByteArray(0, 10),
            els::except::InvalidArgument);
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, strConstr)
{
    static const std::string str = "ELSELSELSELS";
    els::misc::ByteArray ba(str);
    ELSUNIT_EXPECT_EQ(str.size(), ba.size());
    ELSUNIT_EXPECT_EQ(0, ::memcmp(str.data(), ba.get(), str.size()));
    ELSUNIT_EXPECT_EXCEPTION(els::misc::ByteArray(""),
            els::except::InvalidArgument);
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, copyConstr)
{
    static const char* const data = "ELSELSELSELSELS\t\nELS";
    els::misc::ByteArray b1(data, sizeof(data));
    els::misc::ByteArray b2(b1);
    ELSUNIT_EXPECT_EQ(0, ::memcmp(data, b1.get(), sizeof(data)));
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, assignOp)
{
    static const char* const data = "ELSELSELS\t\nELS";
    els::misc::ByteArray b1(data, sizeof(data));
    els::misc::ByteArray b2;
    ELSUNIT_EXPECT_TRUE(b2.empty());
    b2 = b1;
    ELSUNIT_EXPECT_EQ(0, ::memcmp(data, b1.get(), sizeof(data)));
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, get)
{
    static const char* const data = "ELSELS";
    els::misc::ByteArray ba(data, sizeof(data));
    ELSUNIT_EXPECT_EQ(0, ::memcmp(ba.get(), data, sizeof(data)));
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, set)
{
    static const char* const data = "ELSsomethinELS";
    els::misc::ByteArray b(sizeof(data));
    ELSUNIT_EXPECT_NO_THROW(b.set(data, sizeof(data)));
    ELSUNIT_EXPECT_EQ(0, ::memcmp(data, b.get(), sizeof(data)));
    ELSUNIT_EXPECT_EXCEPTION(b.set(0, 10), els::except::InvalidArgument);
    ELSUNIT_EXPECT_EXCEPTION(b.set(data, 0), els::except::InvalidArgument);
    ELSUNIT_EXPECT_EXCEPTION(b.set(data, 100), els::except::InvalidArgument);
}

ELSUNIT_SIMPLE_TESTCASE(ByteArray, toStr)
{
    static const char* const buf = "somethin somethin";
    els::misc::ByteArray array(buf, ::strlen(buf));
    ELSUNIT_EXPECT_STRING_EQ(std::string("somethin somethin"), array.toStr());
}


