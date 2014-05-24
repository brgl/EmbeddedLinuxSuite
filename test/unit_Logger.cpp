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
 * @file    unit_Logger.cpp
 */

#include "ElsUnit.hpp"

#include <els/Logger.hpp>
#include <els/ILogHandler.hpp>

namespace {

unsigned counter = 0;

class TestLogHandler : public els::log::ILogHandler
{
public:
    TestLogHandler(void) : els::log::ILogHandler() {}
    virtual ~TestLogHandler(void) throw() {}

    virtual void log(els::log::LogLevel level, const char* msg) { counter++; }
private:
    ELS_CLASS_UNCOPYABLE(TestLogHandler);
};

}

ELSUNIT_SIMPLE_TESTCASE(Logger, logLevel)
{
    els::log::Logger logger;
    TestLogHandler logHandler;

    counter = 0;
    ELSUNIT_EXPECT_EQ(0, counter);
    ELSUNIT_ASSERT_NO_THROW(logger.warn("test"));
    ELSUNIT_EXPECT_EQ(0, counter);
    ELSUNIT_ASSERT_NO_THROW(logger.addLogHandler(&logHandler, false));
    ELSUNIT_ASSERT_NO_THROW(logger.warn("test"));
    ELSUNIT_ASSERT_NO_THROW(logger.info("test"));
    ELSUNIT_EXPECT_EQ(1, counter);
    ELSUNIT_ASSERT_NO_THROW(logger.setLogLevel(els::log::ELS_LOG_DEBUG));
    ELSUNIT_ASSERT_NO_THROW(logger.error("test"));
    ELSUNIT_ASSERT_NO_THROW(logger.info("test"));
    ELSUNIT_EXPECT_EQ(3, counter);
}

