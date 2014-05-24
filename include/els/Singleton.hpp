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
 * @file    Singleton.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Mutex.hpp"
#include "AutoMutex.hpp"

#include <memory>

ELS_BEGIN_NAMESPACE_2(els, misc)

/**
 * @brief   Template allowing easy definition of singleton classes.
 */
template<class T> class ELS_EXPORT_SYMBOL Singleton
{
public:

    static T& instance(void);

private:

    static std::auto_ptr<T> _S_instance;
    static thread::Mutex _S_mutex;

    ELS_CLASS_NOT_INSTANTIABLE(Singleton<T>);
};

template<class T> std::auto_ptr<T> Singleton<T>::_S_instance(0);
template<class T> thread::Mutex Singleton<T>::_S_mutex;

/**
 * @brief   Returns a reference to the single instance of the singleton
 *          object. Creates the instance if called for the first time.
 * @return  Reference to the singleton instance.
 *
 * It uses double-checked locking pattern. As it is not really thread-safe
 * on SMP processors it will be probably changed to using spinlocks
 * in the future.
 */
template<class T> T& Singleton<T>::instance(void)
{
    if (_S_instance.get() == 0)
    {
        thread::AutoMutex am(_S_mutex);

        if (_S_instance.get() == 0)
            _S_instance.reset(new T);
    }

    return *(_S_instance);
}

ELS_END_NAMESPACE_2

