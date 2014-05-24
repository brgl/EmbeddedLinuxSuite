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
 * @file    unit_System.cpp
 */

#include "ElsUnit.hpp"

#include <els/System.hpp>

#include <unistd.h>
#include <syscall.h>

ELSUNIT_SIMPLE_TESTCASE(System, getpid)
{
    ELSUNIT_EXPECT_EQ(::getpid(), els::sys::getPid());
}

ELSUNIT_SIMPLE_TESTCASE(System, gettid)
{
    ELSUNIT_EXPECT_EQ(::syscall(SYS_gettid), els::sys::getTid());
}


