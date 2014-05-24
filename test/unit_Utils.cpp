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
 * @file    unit_Utils.cpp
 */

#include "ElsUnit.hpp"

#include <els/Utils.hpp>
#include <els/Types.hpp>

ELSUNIT_SIMPLE_TESTCASE(Utils, randInt)
{
    els::ElsInt32 r1, r2, r3;
    r1 = els::misc::utils::randomInt();
    r2 = els::misc::utils::randomInt();
    r3 = els::misc::utils::randomInt();
    ELSUNIT_EXPECT_TRUE((r1 != r2) && (r2 != r3));
}

ELSUNIT_SIMPLE_TESTCASE(Utils, randUint)
{
    els::ElsUint32 r1, r2, r3;
    r1 = els::misc::utils::randomUint();
    r2 = els::misc::utils::randomUint();
    r3 = els::misc::utils::randomUint();
    ELSUNIT_EXPECT_TRUE((r1 != r2) && (r2 != r3));
}

