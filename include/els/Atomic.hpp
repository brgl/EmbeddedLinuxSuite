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
 * @file    Atomic.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"

ELS_BEGIN_NAMESPACE_2(els, thread)

/**
 * @brief   Allows atomic access and modification of integral variables.
 */
template <class T> class ELS_EXPORT_SYMBOL Atomic
{
public:

    /**
     * @brief   Constructor. Initializes the internal atomic variable.
     * @param   i       Initial value.
     */
    explicit Atomic(T i) throw() : _M_i(i) {}

    /**
     * @brief   Destructor.
     */
    ~Atomic(void) throw() {}

    /**
     * @brief   Sets the internal atomic integer to a new value.
     * @param   i       New value.
     */
    void set(T i) throw() { (void)::__sync_lock_test_and_set(&this->_M_i, i); }

    /**
     * @brief   Returns current value of the atomic integer.
     * @return  Current value.
     */
    T get(void) throw() { return ::__sync_fetch_and_or(&this->_M_i, 0); }

    /**
     * @brief   Increments the internal atomic integer by one and
     *          returns its value.
     * @return  Value of the atomic integer after incrementation.
     */
    T inc(void) throw() { return ::__sync_add_and_fetch(&this->_M_i, 1); }

    /**
     * @brief   Decrements the internal atomic integer by one and
     *          returns its value.
     * @return  Value of the atomic integer after decrementation.
     */
    T dec(void) throw() { return ::__sync_sub_and_fetch(&this->_M_i, 1); }

private:

    volatile T _M_i;

    ELS_CLASS_UNCOPYABLE(Atomic<T>);
};

typedef Atomic<ElsInt32> AtomicInt;
typedef Atomic<ElsUint32> AtomicUint;

ELS_END_NAMESPACE_2

