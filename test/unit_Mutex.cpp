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
 * @file    unit_Mutex.cpp
 */

#include "ElsUnit.hpp"

#include <els/IThread.hpp>
#include <els/Mutex.hpp>

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
        mutex.lock();
        counter = 10;
        mutex.unlock();
        return 0;
    }
};

}

ELSUNIT_SIMPLE_TESTCASE(Mutex, lock)
{
    Thread thread;

    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
    counter = 20;
    ELSUNIT_ASSERT_NO_THROW(mutex.lock());
    ELSUNIT_ASSERT_NO_THROW(thread.start());
    ELSUNIT_EXPECT_EQ(20, counter);
    ELSUNIT_ASSERT_NO_THROW(mutex.unlock());
    ELSUNIT_ASSERT_NO_THROW(thread.join());
    ELSUNIT_EXPECT_EQ(10, counter);
    ELSUNIT_EXPECT_EQ(0, els::thread::IThread::threadCount());
}

ELSUNIT_SIMPLE_TESTCASE(Mutex, trylock)
{
    els::thread::Mutex mtx;
    ELSUNIT_ASSERT_NO_THROW(mtx.lock());
    ELSUNIT_ASSERT_NO_THROW(ELSUNIT_EXPECT_FALSE(mtx.trylock()));
    ELSUNIT_ASSERT_NO_THROW(mtx.unlock());
    ELSUNIT_ASSERT_NO_THROW(ELSUNIT_EXPECT_TRUE(mtx.trylock()));
    ELSUNIT_ASSERT_NO_THROW(mtx.unlock());
}

