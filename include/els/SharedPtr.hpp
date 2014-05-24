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
 * @file    SharedPtr.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include "Atomic.hpp"
#include "Exception.hpp"

ELS_BEGIN_NAMESPACE_2(els, misc)

ELS_BEGIN_NAMESPACE_1(__shared_ptr_detail)

class __SharedPtrRefs
{
public:

    ELS_EXPORT_SYMBOL __SharedPtrRefs(void);
    ELS_EXPORT_SYMBOL ~__SharedPtrRefs(void);

    ELS_EXPORT_SYMBOL ElsInt32 incStrong(void);
    ELS_EXPORT_SYMBOL ElsInt32 incWeak(void);
    ELS_EXPORT_SYMBOL ElsInt32 decStrong(void);
    ELS_EXPORT_SYMBOL ElsInt32 decWeak(void);
    ELS_EXPORT_SYMBOL ElsBool hasStrong(void);
    ELS_EXPORT_SYMBOL ElsBool hasWeak(void);

private:

    thread::AtomicInt _M_strong;
    thread::AtomicInt _M_weak;

    ELS_CLASS_UNCOPYABLE(__SharedPtrRefs);
};

ELS_END_NAMESPACE_1

template <typename T> class WeakPtr;

template <typename T> class ELS_EXPORT_SYMBOL SharedPtr
{
public:

    SharedPtr(void)
        : _M_ptr(0),
          _M_refs(0)
    {

    }

    explicit SharedPtr(T* ptr)
        : _M_ptr(ptr),
          _M_refs(new __shared_ptr_detail::__SharedPtrRefs)
    {

    }

    SharedPtr(const SharedPtr<T>& other)
        : _M_ptr(other._M_ptr),
          _M_refs(other._M_refs)
    {
        if (this->_M_refs != 0)
            this->_M_refs->incStrong();
    }

    SharedPtr<T>& operator =(const SharedPtr<T>& other)
    {
        this->_M_unref();
        this->_M_ptr = other._M_ptr;
        this->_M_refs = other._M_refs;
        if (this->_M_refs != 0)
            this->_M_refs->incStrong();
        return *this;
    }

    ~SharedPtr(void)
    {
        this->_M_unref();
    }

    T* get(void) const
    {
        return this->_M_ptr;
    }

    void reset(T* ptr)
    {
        this->_M_unref();
        if (ptr != 0)
        {
            this->_M_ptr = ptr;
            this->_M_refs = new __shared_ptr_detail::__SharedPtrRefs;
        }
    }

    T& operator *(void) const
    {
        return *this->_M_ptr;
    }

    T* operator ->(void) const
    {
        return this->_M_ptr;
    }

    operator bool(void) const
    {
        return (this->_M_ptr != 0);
    }

private:

    void _M_unref(void)
    {
        if (this->_M_refs->decStrong() == 0)
        {
            if (!this->_M_refs->hasWeak())
                delete this->_M_refs;

            delete this->_M_ptr;
        }

        this->_M_ptr = 0;
        this->_M_refs = 0;
    }

    T* _M_ptr;
    __shared_ptr_detail::__SharedPtrRefs* _M_refs;

    friend class WeakPtr<T>;
};

ELS_DECLARE_EXCEPTION(SharedPtrExpired, except::Exception);

template <typename T> class ELS_EXPORT_SYMBOL WeakPtr
{
public:

    WeakPtr(void)
        : _M_ptr(0),
          _M_refs(0)
    {

    }

    explicit WeakPtr(const SharedPtr<T>& shared)
        : _M_ptr(shared._M_ptr),
          _M_refs(shared._M_refs)
    {
        if (this->_M_refs != 0)
            this->_M_refs->incWeak();
    }

    WeakPtr(const WeakPtr<T>& other)
        : _M_ptr(other._M_ptr),
          _M_refs(other._M_refs)
    {
        if (this->_M_refs != 0)
            this->_M_refs->incWeak();
    }

    WeakPtr& operator =(const WeakPtr& other)
    {
        this->_M_unref();
        this->_M_ptr = other._M_ptr;
        this->_M_refs = other._M_refs;
        if (this->_M_refs != 0)
            this->_M_refs->incWeak();
        return *this;
    }

    ~WeakPtr(void)
    {
        this->_M_unref();
    }

    bool expired(void) const
    {
        return (!this->_M_refs->hasStrong());
    }

    SharedPtr<T> toSharedPtr(void) const
    {
        SharedPtr<T> ret;

        if (this->_M_refs->incStrong() == 1)
            throw SharedPtrExpired("Shared pointer expired");
        ret._M_ptr = this->_M_ptr;
        ret._M_refs = this->_M_refs;
        return ret;
    }

private:

    void _M_unref(void)
    {
        if (this->_M_refs->decWeak() == 0)
        {
            if (!this->_M_refs->hasStrong())
                delete this->_M_refs;
        }

        this->_M_ptr = 0;
        this->_M_refs = 0;
    }

    T* _M_ptr;
    __shared_ptr_detail::__SharedPtrRefs* _M_refs;
};

ELS_END_NAMESPACE_2

