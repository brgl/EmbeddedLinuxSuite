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
 * @file    Condition.cpp
 */

#include <els/Condition.hpp>

#include <ctime>
#include <cerrno>

ELS_BEGIN_NAMESPACE_2(els, thread)

/**
 * @brief   Default constructor. Initializes the internal
 *          pthread_cond_t object.
 * @throw   ConditionError  Error in initialization.
 */
Condition::Condition(void)
    : _M_cond(),
      _M_mutex()
{
    ::pthread_mutexattr_t attr;
    int ret = 0;

    ::pthread_mutexattr_init(&attr);
    ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
    ret = ::pthread_mutex_init(&this->_M_mutex, &attr);
    ::pthread_mutexattr_destroy(&attr);
    if (ret != 0)
        throw ConditionError(
                "Error initiating internal mutex: %s",
                except::getErrnoStr(ret).c_str());

    ret = ::pthread_cond_init(&this->_M_cond, 0);
    if (ret != 0)
        throw ConditionError(
                "Error initiating condition variable: %s",
                except::getErrnoStr(ret).c_str());
}

/**
 * @brief   Destructor.
 */
Condition::~Condition(void) throw()
{
    ::pthread_mutex_destroy(&this->_M_mutex);
    ::pthread_cond_destroy(&this->_M_cond);
}

/**
 * @brief   Blocks current thread until it's unblocked by another.
 * @throw   ConditionError  If something goes wrong with internal
 *                          pthread function calls.
 * @throw   MutexError      Error locking the mutex.
 */
void Condition::block(void)
{
    int ret = 0;

    this->_M_lockMutex();
    ret = ::pthread_cond_wait(&this->_M_cond, &this->_M_mutex);
    if (ret != 0)
    {
        this->_M_unlockMutex();
        throw ConditionError(
                "Error locking on condition variable: %s",
                except::getErrnoStr(ret).c_str());
    }
    this->_M_unlockMutex();
}

/**
 * @brief   Blocks current thread until it's unblocked by another
 *          or specified timeout expires.
 * @param   tv              Time after which thread will wake up.
 * @param   throwOnTimeout  Indicates whether an exception should be thrown
 *                          after reaching the timeout.
 * @throw   ConditionError  If something goes wrong with internal
 *                          pthread function calls.
 * @throw   MutexError      Error locking the mutex.
 * @throw   CondTimedout    Timeout passed and throwOnTimeout was set to true.
 */
void Condition::block(const misc::Timeval& tv, bool throwOnTimeout)
{
    int ret = 0;
    ::timespec ts;

    tv.toTimespec(ts);
    this->_M_lockMutex();
    ret = :: pthread_cond_timedwait(&this->_M_cond, &this->_M_mutex, &ts);
    if (ret != 0)
    {
        this->_M_unlockMutex();
        if (ret == ETIMEDOUT)
        {
            if (throwOnTimeout)
                throw CondTimedOut("Conditional wait timed out");
            else
                return;
        }
        throw ConditionError(
                "Error locking on condition variable: %s",
                except::getErrnoStr(ret).c_str());
    }
    this->_M_unlockMutex();
}

/**
 * @brief   Unblocks a single thread blocked on this condition variable.
 *          Internal pthread mechanisms decide which thread shall
 *          be unlocked.
 * @throw   ConditionError  If unblocking fails for some reason.
 * @throw   MutexError      Error unlocking the mutex.
 */
void Condition::unblockOne(void)
{
    int ret = 0;

    ret = ::pthread_cond_signal(&this->_M_cond);
    if (ret != 0)
        throw ConditionError(
                "Error unblocking a single thread: %s",
                except::getErrnoStr(ret).c_str());
}

/**
 * @brief   Unblocks all threads blocked on this condition variable.
 * @throw   ConditionError  If unblocking fails for some reason.
 * @throw   MutexError      Error unlocking the mutex.
 */
void Condition::unblockAll(void)
{
    int ret = 0;

    ret = ::pthread_cond_broadcast(&this->_M_cond);
    if (ret != 0)
        throw ConditionError(
                "Error unblocking all blocked threads: %s",
                except::getErrnoStr(ret).c_str());
}

inline void Condition::_M_lockMutex(void)
{
    int ret = 0;

    ret = ::pthread_mutex_lock(&this->_M_mutex);
    if (ret != 0)
        throw ConditionError(
                "Error locking internal mutex: %s",
                except::getErrnoStr(ret).c_str());
}

inline void Condition::_M_unlockMutex(void)
{
    int ret = 0;

    ret = ::pthread_mutex_unlock(&this->_M_mutex);
    if (ret != 0)
        throw ConditionError(
                "Error unlocking internal mutex: %s",
                except::getErrnoStr(ret).c_str());
}

ELS_DEFINE_NESTED_EXCEPTION(ConditionError, Condition, except::Exception);
ELS_DEFINE_NESTED_EXCEPTION(CondTimedOut, Condition, except::Exception);

ELS_END_NAMESPACE_2

