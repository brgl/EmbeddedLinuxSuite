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
 * @file    ReadWriteLock.cpp
 */

#include <els/ReadWriteLock.hpp>

#include <cerrno>

ELS_BEGIN_NAMESPACE_2(els, thread)

ReadWriteLock::ReadWriteLock(void)
    : _M_rwlock()
{
    int ret = 0;

    ret = ::pthread_rwlock_init(&this->_M_rwlock, 0);
    if (ret != 0)
        throw ReadWriteLockError(
                "Error initiating read-write lock: %s",
                except::getErrnoStr(ret).c_str());
}

ReadWriteLock::~ReadWriteLock(void) throw()
{
    ::pthread_rwlock_destroy(&this->_M_rwlock);
}

void ReadWriteLock::rdlock(void)
{
    int ret = ::pthread_rwlock_rdlock(&this->_M_rwlock);
    if (ret != 0)
        throw ReadWriteLockError(
                "Error locking read/write lock for reading: %s",
                except::getErrnoStr(ret).c_str());
}

bool ReadWriteLock::tryrdlock(void)
{
    int ret = ::pthread_rwlock_tryrdlock(&this->_M_rwlock);
    if (ret != 0)
    {
        if (ret == EBUSY)
            return false;

        throw ReadWriteLockError(
                "Error trylocking read/write lock for reading: %s",
                except::getErrnoStr(ret).c_str());
    }

    return true;
}

void ReadWriteLock::wrlock(void)
{
    int ret = ::pthread_rwlock_wrlock(&this->_M_rwlock);
    if (ret != 0)
        throw ReadWriteLockError(
                "Error locking read/write lock for writing: %s",
                except::getErrnoStr(ret).c_str());
}

bool ReadWriteLock::trywrlock(void)
{
    int ret = ::pthread_rwlock_trywrlock(&this->_M_rwlock);
    if (ret != 0)
    {
        if (ret == EBUSY)
            return false;

        throw ReadWriteLockError(
                "Error trylocking read/write lock for writing: %s",
                except::getErrnoStr(ret).c_str());
    }

    return true;
}

void ReadWriteLock::unlock(void)
{
    int ret = ::pthread_rwlock_unlock(&this->_M_rwlock);
    if (ret != 0)
        throw ReadWriteLockError(
                "Error unlocking read/write lock: %s",
                except::getErrnoStr(ret).c_str());
}

ELS_DEFINE_NESTED_EXCEPTION(ReadWriteLockError,
        ReadWriteLock, except::Exception);

ELS_END_NAMESPACE_2


