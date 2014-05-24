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
 * @file    Mutex.cpp
 */

#include <els/Mutex.hpp>

#include <cerrno>

ELS_BEGIN_NAMESPACE_2(els, thread)

/**
 * @brief   Default Constructor. It initializes the internal pthread
 *          mutex object and sets its attributes.
 * @throw   MutexError    If the initialization fails for some reason
 */
Mutex::Mutex(void)
    : _M_mutex()
{
    ::pthread_mutexattr_t attr;
    int ret = 0;

    ret = ::pthread_mutexattr_init(&attr);
    if (ret != 0)
        throw MutexError("Error initiating mutex attribute: %s",
                except::getErrnoStr(ret).c_str());
    ret = ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
    if (ret != 0)
        throw MutexError("Error setting mutex type: %s",
                except::getErrnoStr(ret).c_str());
    ret = ::pthread_mutex_init(&this->_M_mutex, &attr);
    ::pthread_mutexattr_destroy(&attr);
    if (ret != 0)
        throw MutexError("Error initiating mutex: %s",
                except::getErrnoStr(ret).c_str());
}

/**
 * @brief   Destructor. Destroys the internal pthread mutex.
 */
Mutex::~Mutex(void) throw()
{
    ::pthread_mutex_destroy(&this->_M_mutex);
}

/**
 * @brief   It acquires the lock if the mutex isn't already taken.
 *          Otherwise the function blocks until the mutex is freed.
 * @throw   MutexError    If acquiring the mutex fails.
 */
void Mutex::lock(void)
{
    int ret = ::pthread_mutex_lock(&this->_M_mutex);
    if (ret != 0)
        throw MutexError("Error locking mutex: %s",
                except::getErrnoStr(ret).c_str());
}

/**
 * @brief   Acquires the lock if it's not already taken. Otherwise
 *          it does not block like lock(), but exits immediately.
 * @return  True if the lock has been succesfully acuired.
 *          False if the mutex is already locked.
 * @throw   MutexError    If the internal trylock function fails.
 */
bool Mutex::trylock(void)
{
    int ret = ::pthread_mutex_trylock(&this->_M_mutex);
    if (ret != 0)
    {
        if (ret == EBUSY)
            return false;

        throw MutexError("Error trylocking mutex: %s",
                except::getErrnoStr(ret).c_str());
    }

    return true;
}

/**
 * @brief   Frees the previously taken lock.
 * @throw   MutexError    If unlocking fails.
 */
void Mutex::unlock(void)
{
    int ret = ::pthread_mutex_unlock(&this->_M_mutex);
    if (ret != 0)
        throw MutexError("Error unlocking mutex: %s",
                except::getErrnoStr(ret).c_str());
}

ELS_DEFINE_NESTED_EXCEPTION(MutexError, Mutex, except::Exception)

ELS_END_NAMESPACE_2


