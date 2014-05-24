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
 * @file    System.hpp
 * @brief   C++ wrappers for various system calls and libc library calls.
 *
 * These throw LibcError exceptions in case of errors.
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"

#include <string>

ELS_BEGIN_NAMESPACE_2(els, sys)

ELS_EXPORT_SYMBOL pid_t getPid(void) throw();
ELS_EXPORT_SYMBOL pid_t getTid(void) throw();
ELS_EXPORT_SYMBOL char** getEnvp(void) throw();
ELS_EXPORT_SYMBOL void makeDir(const std::string& path, ElsMode mode);
ELS_EXPORT_SYMBOL void* libcMalloc(ElsSize size);
ELS_EXPORT_SYMBOL void libcFree(void* ptr);

ELS_END_NAMESPACE_2

