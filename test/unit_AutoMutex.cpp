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
 * @file    unit_AutoMutex.cpp
 */

#include "ElsUnit.hpp"

#include <els/Mutex.hpp>
#include <els/AutoMutex.hpp>
#include <els/IThread.hpp>

namespace {

volatile int counter = 0;
els::thread::Mutex mutex;

class Thread : public els::thread::IThread
{
public:
    Thread(void) : els::thread::IThread() {}
    virtual ~Thread(void) {}
protected:
    virtual int _M_run(void)
    {
        els::thread::AutoMutex am(mutex);
        counter = 10;
        return 0;
    }
};

}

ELSUNIT_SIMPLE_TESTCASE(AutoMutex, autoLock)
{
    Thread thread;

    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
    counter = 20;
    {
        els::thread::AutoMutex am(mutex);
        ELSUNIT_ASSERT_NO_THROW(thread.start());
        ELSUNIT_EXPECT_EQ(20, counter);
    }
    thread.join();
    ELSUNIT_EXPECT_EQ(10, counter);
    ELSUNIT_EXPECT_EQ(0, els::thread::IThread::threadCount());
}

