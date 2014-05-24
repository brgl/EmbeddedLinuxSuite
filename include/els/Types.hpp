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
 * @file    Types.hpp
 *
 * Various fixed-size types for ELS.
 */

#pragma once

#include "Macros.hpp"

#include <stdint.h>
#include <cstdlib>
#include <sys/types.h>
#include <ctime>

ELS_BEGIN_NAMESPACE_1(els)

typedef int16_t ElsInt16;
typedef int32_t ElsInt32;
typedef int64_t ElsInt64;

typedef uint16_t ElsUint16;
typedef uint32_t ElsUint32;
typedef uint64_t ElsUint64;

typedef uint8_t ElsByte;
typedef bool ElsBool;
typedef size_t ElsSize;

typedef mode_t ElsMode;
typedef time_t ElsTime;

ELS_END_NAMESPACE_1

