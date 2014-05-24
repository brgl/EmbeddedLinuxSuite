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
 * @file    unit_SoLoader.cpp
 */

#include "ElsUnit.hpp"

#include <els/SoLoader.hpp>

ELSUNIT_SIMPLE_TESTCASE(SoLoader, notOpened)
{
    els::misc::SoLoader sl;
    ELSUNIT_EXPECT_EXCEPTION(sl.getSymbol("somethin"),
            els::except::LogicError);
}

ELSUNIT_SIMPLE_TESTCASE(SoLoader, errorOpening)
{
    static const char* const path = "/non-existent/non-existent";
    els::misc::SoLoader sl;
    ELSUNIT_EXPECT_EXCEPTION(sl.open(path),
            els::misc::SoLoader::SoLoaderError);
    ELSUNIT_EXPECT_EXCEPTION(els::misc::SoLoader sl(path),
            els::misc::SoLoader::SoLoaderError);
}

ELSUNIT_SIMPLE_TESTCASE(SoLoader, closeNothing)
{
    els::misc::SoLoader sl;
    ELSUNIT_EXPECT_NO_THROW(sl.close());
}


