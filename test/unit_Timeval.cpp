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
 * @file    unit_Timeval.cpp
 */

#include "ElsUnit.hpp"

#include <ctime>

#include <els/Timeval.hpp>
#include <els/Exception.hpp>

ELSUNIT_SIMPLE_TESTCASE(Timeval, invalidArgument)
{
    els::misc::Timeval t;

    ELSUNIT_EXPECT_EXCEPTION(
            t.setNsec(1000000001),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_EXCEPTION(
            els::misc::Timeval(0, 1000000001),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_NO_THROW(els::misc::Timeval(0, 1000));
}

ELSUNIT_SIMPLE_TESTCASE(Timeval, toTimeval)
{
    els::misc::Timeval t(500, 123456789);
    ::timeval tv;
    t.toTimeval(tv);
    ELSUNIT_EXPECT_EQ(500, tv.tv_sec);
    ELSUNIT_EXPECT_EQ(123456, tv.tv_usec);
}

ELSUNIT_SIMPLE_TESTCASE(Timeval, toTimespec)
{
    els::misc::Timeval t(500, 123456789);
    ::timespec ts;
    t.toTimespec(ts);
    ELSUNIT_EXPECT_EQ(500, ts.tv_sec);
    ELSUNIT_ASSERT_EQ(123456789, ts.tv_nsec);
}


