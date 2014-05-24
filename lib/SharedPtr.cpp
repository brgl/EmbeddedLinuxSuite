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
 * @file    SharedPtr.cpp
 */

#include <els/SharedPtr.hpp>

ELS_BEGIN_NAMESPACE_3(els, misc, __shared_ptr_detail)

__SharedPtrRefs::__SharedPtrRefs(void)
    : _M_strong(1),
      _M_weak(0)
{

}

__SharedPtrRefs::~__SharedPtrRefs(void)
{

}

ElsInt32 __SharedPtrRefs::incStrong(void)
{
    return this->_M_strong.inc();
}

ElsInt32 __SharedPtrRefs::incWeak(void)
{
    return this->_M_weak.inc();
}

ElsInt32 __SharedPtrRefs::decStrong(void)
{
    return this->_M_strong.dec();
}

ElsInt32 __SharedPtrRefs::decWeak(void)
{
    return this->_M_weak.dec();
}

ElsBool __SharedPtrRefs::hasStrong(void)
{
    return (this->_M_strong.get() > 0);
}

ElsBool __SharedPtrRefs::hasWeak(void)
{
    return (this->_M_weak.get() > 0);
}

ELS_END_NAMESPACE_3

ELS_BEGIN_NAMESPACE_2(els, misc
        )
ELS_DEFINE_EXCEPTION(SharedPtrExpired, except::Exception);

ELS_END_NAMESPACE_2


