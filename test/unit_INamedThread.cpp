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
 * @file    unit_INamedThread.cpp
 */

#include "ElsUnit.hpp"

#include <els/INamedThread.hpp>

#include <string>

static std::string threadName;

class NamedThread : public els::thread::INamedThread
{
public:
    NamedThread(const std::string& name) : els::thread::INamedThread(name) {}
protected:
    virtual int _M_run(void)
    {
        threadName = this->name();
        return 0;
    }
};

ELSUNIT_SIMPLE_TESTCASE(INamedThread, name)
{
    NamedThread thread("NAME");

    threadName = "none";
    ELSUNIT_EXPECT_STRING_EQ("none", threadName);
    ELSUNIT_EXPECT_NO_THROW(thread.start());
    ELSUNIT_EXPECT_NO_THROW(thread.join());
    ELSUNIT_EXPECT_STRING_EQ("NAME", threadName);
    ELSUNIT_EXPECT_EQ(0, thread.retval().retval());
}


