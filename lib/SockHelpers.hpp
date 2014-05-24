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
 * @file    SockHelpers.hpp
 */

#pragma once

#include <els/Macros.hpp>
#include <els/Types.hpp>
#include <els/Timeval.hpp>

#include <string>

ELS_BEGIN_NAMESPACE_2(els, sock)

ElsUint32 ipFromStr(const std::string& addr);
bool sockCanRead(int sock, const misc::Timeval& tv);
bool sockCanWrite(int sock, const misc::Timeval& tv);

ELS_END_NAMESPACE_2


