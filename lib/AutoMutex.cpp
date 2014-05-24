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
 * @file    AutoMutex.cpp
 */

#include <els/AutoMutex.hpp>

ELS_BEGIN_NAMESPACE_2(els, thread)

/**
 * @brief   The constructor. It immediately locks the mutex
 *          at construction.
 * @param   mutexRef    Reference to a mutex object.
 * @throw   MutexError  Propagated from Mutex::lock().
 */
AutoMutex::AutoMutex(Mutex& mutexRef)
    : _M_mutexRef(mutexRef)
{
    this->_M_mutexRef.lock();
}

/**
 * @brief   The destructor. It releases the mutex upon destruction
 *          of the object. MutexError thrown by Mutex::unlock()
 *          will be ignored if caught.
 */
AutoMutex::~AutoMutex(void) throw()
{
    try { this->_M_mutexRef.unlock(); } catch (...) {}
}

ELS_END_NAMESPACE_2
