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
 * @file    unit_AtomicInt.cpp
 */

#include "ElsUnit.hpp"

#include <els/Atomic.hpp>

ELSUNIT_SIMPLE_TESTCASE(AtomicInt, get)
{
    els::thread::AtomicInt i(-100);
    ELSUNIT_EXPECT_EQ(-100, i.get());
    ELSUNIT_EXPECT_NOT_EQ(-101, i.get());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicInt, set)
{
    els::thread::AtomicInt i(100);
    i.set(150);
    ELSUNIT_EXPECT_EQ(150, i.get());
    ELSUNIT_EXPECT_NOT_EQ(100, i.get());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicInt, inc)
{
    els::thread::AtomicInt i(100);
    ELSUNIT_EXPECT_EQ(101, i.inc());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicInt, dec)
{
    els::thread::AtomicInt i(100);
    ELSUNIT_EXPECT_EQ(99, i.dec());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicUint, get)
{
    els::thread::AtomicUint i(100);
    ELSUNIT_EXPECT_EQ(100, i.get());
    ELSUNIT_EXPECT_NOT_EQ(101, i.get());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicUint, set)
{
    els::thread::AtomicUint i(100);
    i.set(150);
    ELSUNIT_EXPECT_EQ(150, i.get());
    ELSUNIT_EXPECT_NOT_EQ(100, i.get());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicUint, inc)
{
    els::thread::AtomicUint i(100);
    ELSUNIT_EXPECT_EQ(101, i.inc());
}

ELSUNIT_SIMPLE_TESTCASE(AtomicUint, dec)
{
    els::thread::AtomicUint i(100);
    ELSUNIT_EXPECT_EQ(99, i.dec());
}



