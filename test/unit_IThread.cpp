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
 * @file    unit_IThread.cpp
 */

#include "ElsUnit.hpp"

#include <els/IThread.hpp>
#include <els/Exception.hpp>

#include <unistd.h>
#include <cerrno>

static int valueToChange = 0;

class BasicThread : public els::thread::IThread
{
public:
    BasicThread(void) : els::thread::IThread() {}
protected:
    virtual int _M_run(void)
    {
        valueToChange = 123;
        return 0;
    }
};

class LoopThread : public els::thread::IThread
{
public:
    LoopThread(void) : els::thread::IThread() {}
protected:
    virtual int _M_run(void)
    {
        valueToChange = 111;
        while (!this->_M_stopRequested()) ::usleep(10000);
        valueToChange = 123;
        return 0;
    }
};

class ExceptionThread : public els::thread::IThread
{
public:
    ExceptionThread(void) : els::thread::IThread() {}
protected:
    virtual int _M_run(void)
    {
        throw els::except::LibcError(EINVAL, "EINVAL");
        return 0;
    }
};

ELSUNIT_SIMPLE_TESTCASE(IThread, basicThread)
{
    BasicThread thread;

    valueToChange = 321;
    ELSUNIT_EXPECT_EQ(321, valueToChange);
    ELSUNIT_EXPECT_NO_THROW(thread.start());
    ELSUNIT_EXPECT_NO_THROW(thread.join());
    ELSUNIT_EXPECT_EQ(123, valueToChange);
    ELSUNIT_EXPECT_EQ(0, thread.retval().retval());
}

ELSUNIT_SIMPLE_TESTCASE(IThread, loopThread)
{
    LoopThread thread;

    valueToChange = 321;
    ELSUNIT_EXPECT_EQ(321, valueToChange);
    ELSUNIT_EXPECT_NO_THROW(thread.start());
    ::usleep(100000);
    ELSUNIT_EXPECT_EQ(111, valueToChange);
    ELSUNIT_EXPECT_NO_THROW(thread.stop());
    ELSUNIT_EXPECT_NO_THROW(thread.join());
    ELSUNIT_EXPECT_EQ(123, valueToChange);
    ELSUNIT_EXPECT_EQ(0, thread.retval().retval());
}

ELSUNIT_SIMPLE_TESTCASE(IThread, exceptionThread)
{
    ExceptionThread thread;

    ELSUNIT_EXPECT_NO_THROW(thread.start());
    ::usleep(10000);
    ELSUNIT_EXPECT_NO_THROW(thread.stop());
    ELSUNIT_EXPECT_NO_THROW(thread.join());
    ELSUNIT_EXPECT_TRUE(thread.retval().hasError());
    ELSUNIT_EXPECT_STRING_EQ("EINVAL", thread.retval().errMsg());
    ELSUNIT_EXPECT_EQ(-1, thread.retval().retval());
}

