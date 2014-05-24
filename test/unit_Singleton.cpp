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
 * @file    unit_Singleton.cpp
 */

#include "ElsUnit.hpp"

#include <els/Singleton.hpp>

#include <string>

namespace {

class StringContainer
{
public:
    StringContainer(void) : _M_str() {}
    const std::string& get(void) { return this->_M_str; }
    void set(const std::string& str) { this->_M_str = str; }
private:
    std::string _M_str;
};

typedef els::misc::Singleton<StringContainer> SingletonStrContainer;

std::string lazy;

class Lazy
{
public:
    Lazy(void) { lazy = "init"; }
};

typedef els::misc::Singleton<Lazy> LazySingleton;

}

ELSUNIT_SIMPLE_TESTCASE(Singleton, basicTest)
{
    ELSUNIT_EXPECT_TRUE(SingletonStrContainer::instance().get().empty());
    SingletonStrContainer::instance().set("somethin");
    ELSUNIT_EXPECT_STRING_EQ("somethin",
            SingletonStrContainer::instance().get());
}

ELSUNIT_SIMPLE_TESTCASE(Singleton, lazyInit)
{
    lazy = "noinit";
    ELSUNIT_EXPECT_STRING_EQ("noinit", lazy);
    LazySingleton::instance();
    ELSUNIT_EXPECT_STRING_EQ("init", lazy);
}

