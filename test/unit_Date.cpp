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
 * @file    unit_Date.cpp
 */

#include "ElsUnit.hpp"

#include <els/Date.hpp>
#include <els/Exception.hpp>

ELSUNIT_SIMPLE_TESTCASE(Date, fromStr)
{
    els::misc::Date d;

    ELSUNIT_EXPECT_NO_THROW(d.fromStr("2001-11-12 18:31:01",
            "%Y-%m-%d %H:%M:%S"));
    ELSUNIT_EXPECT_EQ(1005586261, d.get());
}

ELSUNIT_SIMPLE_TESTCASE(Date, fromStrBadFmt)
{
    ELSUNIT_EXPECT_EXCEPTION(els::misc::Date("123", "321"),
            els::except::LibcError);
}


