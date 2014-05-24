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
 * @file    Mutex.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Exception.hpp"

#include <pthread.h>

ELS_BEGIN_NAMESPACE_2(els, thread)

/**
 * @brief   Wraps a standard pthread mutex in a C++ class.
 */
class Mutex
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(MutexError, except::Exception);

    ELS_EXPORT_SYMBOL Mutex(void);
    ELS_EXPORT_SYMBOL ~Mutex(void) throw();

    ELS_EXPORT_SYMBOL void lock(void);
    ELS_EXPORT_SYMBOL void unlock(void);
    ELS_EXPORT_SYMBOL bool trylock(void);

private:

    ::pthread_mutex_t _M_mutex;

    ELS_CLASS_UNCOPYABLE(Mutex);
};

ELS_END_NAMESPACE_2

