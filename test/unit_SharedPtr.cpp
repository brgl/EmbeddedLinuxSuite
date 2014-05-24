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
 * @file    unit_SharedPtr.cpp
 */

#include "ElsUnit.hpp"

#include <els/SharedPtr.hpp>

namespace
{

bool constructorCalled = false;
bool destructorCalled = false;

class BasicTestClass
{
public:
    BasicTestClass(void) { constructorCalled = true; }
    ~BasicTestClass(void) { destructorCalled = true; }
};

}

ELSUNIT_SIMPLE_TESTCASE(SharedPtr, basicSharedPtr)
{
    constructorCalled = false;
    destructorCalled = false;
    ELSUNIT_ASSERT_FALSE(constructorCalled);
    ELSUNIT_ASSERT_FALSE(destructorCalled);
    {
        els::misc::SharedPtr<BasicTestClass> i(new BasicTestClass);
        ELSUNIT_EXPECT_TRUE(constructorCalled);
    }
    ELSUNIT_EXPECT_TRUE(destructorCalled);
}


