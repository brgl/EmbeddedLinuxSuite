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
 * @file    ReadWriteLock.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Exception.hpp"

#include <pthread.h>

ELS_BEGIN_NAMESPACE_2(els, thread)

class ReadWriteLock
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(ReadWriteLockError, except::Exception);

    ELS_EXPORT_SYMBOL ReadWriteLock(void);
    ELS_EXPORT_SYMBOL ~ReadWriteLock(void) throw();

    ELS_EXPORT_SYMBOL void rdlock(void);
    ELS_EXPORT_SYMBOL bool tryrdlock(void);
    ELS_EXPORT_SYMBOL void wrlock(void);
    ELS_EXPORT_SYMBOL bool trywrlock(void);
    ELS_EXPORT_SYMBOL void unlock(void);

private:

    ::pthread_rwlock_t _M_rwlock;

    ELS_CLASS_UNCOPYABLE(ReadWriteLock);
};

ELS_END_NAMESPACE_2


