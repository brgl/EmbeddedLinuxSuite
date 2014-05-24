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
 * @file    unit_Condition.cpp
 */

#include "ElsUnit.hpp"

#include <els/Condition.hpp>
#include <els/IThread.hpp>
#include <els/Atomic.hpp>
#include <els/Timeval.hpp>

namespace {

els::thread::AtomicUint counter(0);

class Worker : public els::thread::IThread
{
public:
    explicit Worker(els::thread::Condition* cond)
        : els::thread::IThread(), _M_cond(cond) {}
protected:
    virtual int _M_run(void)
    {
        counter.inc();
        this->_M_cond->block();
        counter.inc();
        return 0;
    }
private:
    els::thread::Condition* _M_cond;
};

}

ELSUNIT_SIMPLE_TESTCASE(Condition, basicBlock)
{
    els::thread::Condition cond;
    Worker worker(&cond);

    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
    counter.set(0);
    ELSUNIT_ASSERT_EQ(0, counter.get());
    ELSUNIT_EXPECT_NO_THROW(worker.start());
    while (counter.get() == 0);
    ELSUNIT_EXPECT_EQ(1, counter.get());
    ELSUNIT_EXPECT_NO_THROW(cond.unblockAll());
    while (counter.get() == 1);
    ELSUNIT_EXPECT_EQ(2, counter.get());
    ELSUNIT_EXPECT_NO_THROW(worker.join());
    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
}

ELSUNIT_SIMPLE_TESTCASE(Condition, timedBlock)
{
    els::thread::Condition cond;

    ELSUNIT_EXPECT_NO_THROW(cond.block(els::misc::Timeval(0, 5000)));
}

ELSUNIT_SIMPLE_TESTCASE(Condition, timedBlockThrow)
{
    els::thread::Condition cond;

    ELSUNIT_EXPECT_EXCEPTION(cond.block(els::misc::Timeval(0, 5000),
            true /* throwOnTimeout */), els::thread::Condition::CondTimedOut);
}

ELSUNIT_SIMPLE_TESTCASE(Condition, unblockOne)
{
    els::thread::Condition cond;
    Worker w1(&cond);
    Worker w2(&cond);
    Worker w3(&cond);

    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
    counter.set(0);
    ELSUNIT_ASSERT_EQ(0, counter.get());
    ELSUNIT_EXPECT_NO_THROW(w1.start());
    ELSUNIT_EXPECT_NO_THROW(w2.start());
    ELSUNIT_EXPECT_NO_THROW(w3.start());
    while (counter.get() != 3);
    ELSUNIT_EXPECT_EQ(3, counter.get());
    ELSUNIT_EXPECT_NO_THROW(cond.unblockOne());
    while (counter.get() == 3);
    ELSUNIT_EXPECT_EQ(4, counter.get());
    ELSUNIT_EXPECT_NO_THROW(cond.unblockOne());
    ELSUNIT_EXPECT_NO_THROW(cond.unblockOne());
    while (counter.get() != 6);
    ELSUNIT_EXPECT_NO_THROW(w1.join());
    ELSUNIT_EXPECT_NO_THROW(w2.join());
    ELSUNIT_EXPECT_NO_THROW(w3.join());
    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
}

ELSUNIT_SIMPLE_TESTCASE(Condition, unblockAll)
{
    els::thread::Condition cond;
    Worker w1(&cond);
    Worker w2(&cond);
    Worker w3(&cond);

    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
    counter.set(0);
    ELSUNIT_ASSERT_EQ(0, counter.get());
    ELSUNIT_EXPECT_NO_THROW(w1.start());
    ELSUNIT_EXPECT_NO_THROW(w2.start());
    ELSUNIT_EXPECT_NO_THROW(w3.start());
    while (counter.get() != 3);
    ELSUNIT_EXPECT_EQ(3, counter.get());
    ELSUNIT_EXPECT_NO_THROW(cond.unblockAll());
    while (counter.get() != 6);
    ELSUNIT_EXPECT_EQ(6, counter.get());
    ELSUNIT_EXPECT_NO_THROW(w1.join());
    ELSUNIT_EXPECT_NO_THROW(w2.join());
    ELSUNIT_EXPECT_NO_THROW(w3.join());
    ELSUNIT_ASSERT_EQ(0, els::thread::IThread::threadCount());
}


