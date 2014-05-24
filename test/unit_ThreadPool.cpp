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
 * @file    unit_ThreadPool.cpp
 */

#include "ElsUnit.hpp"

#include <unistd.h>

#include <els/ThreadPool.hpp>
#include <els/IRunnable.hpp>
#include <els/Macros.hpp>
#include <els/Mutex.hpp>
#include <els/Condition.hpp>

static els::thread::Mutex counterMutex;
static int counter = 0;
static els::thread::Condition cond;

class Incr : public els::thread::IRunnable
{
public:

    Incr(void) : els::thread::IRunnable() {}

    virtual void run(void) throw()
    {
        bool unblock = false;

        ::usleep(100);
        counterMutex.lock();
        counter++;
        unblock = (counter == 100);
        counterMutex.unlock();
        if (unblock)
            cond.unblockAll();
    }
};

ELSUNIT_SIMPLE_TESTCASE(ThreadPool, basicPool)
{
    Incr incr;
    els::thread::ThreadPool pool;

    counter = 0;
    ELSUNIT_EXPECT_NO_THROW(pool.start(10));
    ELSUNIT_EXPECT_EQ(0, counter);
    for (unsigned i = 0; i < 100; ++i)
        pool.schedule(&incr);
    ELSUNIT_EXPECT_NO_THROW(cond.block());
    ELSUNIT_EXPECT_NO_THROW(pool.stop());
    ELSUNIT_EXPECT_EQ(100, counter);
}


