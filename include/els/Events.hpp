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
 * @file    Events.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Atomic.hpp"

#include <set>

ELS_BEGIN_NAMESPACE_2(els, __events)

template <typename T> class ELS_EXPORT_SYMBOL Delegate
{
public:

    Delegate(void)
    {

    }

    virtual ~Delegate(void)
    {

    }

    virtual void operator ()(T& param) = 0;

    ELS_CLASS_UNCOPYABLE(Delegate<T>);
};

struct ELS_EXPORT_SYMBOL ListenerRefs
{
    ListenerRefs(void) : strong(1), weak(0) {}

    thread::AtomicInt strong;
    thread::AtomicInt weak;
};

template <typename T> class ELS_EXPORT_SYMBOL DelegateRef
{
public:

    DelegateRef(Delegate<T>* ptr, ListenerRefs* ref)
        : _M_ptr(ptr),
          _M_refs(ref)
    {

    }

    DelegateRef(const DelegateRef<T>& other)
        : _M_ptr(other._M_ptr),
          _M_refs(other._M_refs)
    {
        this->_M_refs->weak.inc();
    }

    DelegateRef<T>& operator =(const DelegateRef<T>& other)
    {
        other._M_refs->weak.inc();
        this->_M_ptr = other._M_ptr;
        this->_M_refs = other._M_refs;
        return *this;
    }

    ~DelegateRef(void)
    {
        if (this->_M_refs->weak.dec() == 0)
        {
            if (this->_M_refs->strong.get() == 0)
                delete this->_M_refs;
        }
    }

    bool trylock(void) const
    {
        return (this->_M_refs->strong.inc() > 1);
    }

    void unlock(void) const
    {
        this->_M_refs->strong.dec();
    }

    Delegate<T>& get(void) const
    {
        return *(this->_M_ptr);
    }

    bool operator ==(const DelegateRef<T>& other) const
    {
        return this->_M_ptr == other._M_ptr;
    }

    bool operator <(const DelegateRef<T>& other) const
    {
        return this->_M_ptr < other._M_ptr;
    }

private:

    Delegate<T>* _M_ptr;
    mutable ListenerRefs* _M_refs;
};

template <typename T> class ELS_EXPORT_SYMBOL ListenerHandle
{
public:

    ListenerHandle(T* ptr)
        : _M_ptr(ptr),
          _M_refs(new ListenerRefs)
    {

    }

    ~ListenerHandle(void)
    {
        if (this->_M_refs->strong.dec() == 0)
        {
            if (this->_M_refs->weak.get() == 0)
                delete this->_M_refs;
            delete this->_M_ptr;
        }
    }

    operator DelegateRef<typename T::ParamType>(void)
    {
        this->_M_refs->weak.inc();
        return DelegateRef<typename T::ParamType>(
                dynamic_cast<Delegate<typename T::ParamType>* >(
                        this->_M_ptr), this->_M_refs);
    }

private:

    T* _M_ptr;
    ListenerRefs* _M_refs;

    ELS_CLASS_UNCOPYABLE(ListenerHandle<T>);
};

ELS_END_NAMESPACE_2

#define ELS_LISTENER(NAME, OWNER_TYPE, HANDLER_FUNC, PARAM_TYPE)            \
    class __lstn_##NAME##__ : public els::__events::Delegate<PARAM_TYPE>    \
    {                                                                       \
    public:                                                                 \
                                                                            \
        typedef PARAM_TYPE ParamType;                                       \
                                                                            \
        ELS_EXPORT_SYMBOL explicit __lstn_##NAME##__(OWNER_TYPE& owner)     \
            : els::__events::Delegate<PARAM_TYPE>(),                        \
              _M_owner(owner)                                               \
        {                                                                   \
                                                                            \
        }                                                                   \
                                                                            \
        ELS_EXPORT_SYMBOL ~__lstn_##NAME##__(void)                          \
        {                                                                   \
                                                                            \
        }                                                                   \
                                                                            \
        ELS_EXPORT_SYMBOL void operator()(PARAM_TYPE& param)                \
        {                                                                   \
            _M_owner.HANDLER_FUNC(param);                                   \
        }                                                                   \
                                                                            \
    private:                                                                \
                                                                            \
        OWNER_TYPE& _M_owner;                                               \
                                                                            \
        ELS_CLASS_UNCOPYABLE(__lstn_##NAME##__);                            \
    };                                                                      \
    els::__events::ListenerHandle<__lstn_##NAME##__> NAME

#define ELS_INIT_LISTENER(NAME) NAME(new __lstn_##NAME##__(*this))

#define ELS_EVENT(NAME, PARAM_TYPE, OWNER_TYPE)                             \
    class __event_##NAME##__                                                \
    {                                                                       \
    public:                                                                 \
                                                                            \
        typedef els::__events::DelegateRef<PARAM_TYPE> DelegateRef;         \
                                                                            \
    private:                                                                \
                                                                            \
        typedef std::set<DelegateRef> _T_DelegateRefSet;                    \
                                                                            \
    public:                                                                 \
                                                                            \
        __event_##NAME##__(void)                                            \
            : _M_delRefSet()                                                \
        {                                                                   \
                                                                            \
        }                                                                   \
                                                                            \
        ~__event_##NAME##__(void)                                           \
        {                                                                   \
                                                                            \
        }                                                                   \
                                                                            \
        void operator +=(DelegateRef delRef)                                \
        {                                                                   \
            this->_M_delRefSet.insert(delRef);                              \
        }                                                                   \
                                                                            \
        void operator -=(DelegateRef delRef)                                \
        {                                                                   \
            this->_M_delRefSet.erase(delRef);                               \
        }                                                                   \
                                                                            \
    private:                                                                \
                                                                            \
        void operator ()(PARAM_TYPE& param)                                 \
        {                                                                   \
            for (_T_DelegateRefSet::iterator it                             \
                    = this->_M_delRefSet.begin();                           \
                    it != this->_M_delRefSet.end(); ++it)                   \
            {                                                               \
                if (!it->trylock())                                         \
                {                                                           \
                    this->_M_delRefSet.erase((*it));                        \
                }                                                           \
                else                                                        \
                {                                                           \
                    it->get().operator ()(param);                           \
                    it->unlock();                                           \
                }                                                           \
            }                                                               \
        }                                                                   \
                                                                            \
        _T_DelegateRefSet _M_delRefSet;                                     \
                                                                            \
        ELS_CLASS_UNCOPYABLE(__event_##NAME##__);                           \
                                                                            \
        friend class OWNER_TYPE;                                            \
    };                                                                      \
    __event_##NAME##__ NAME

#define ELS_INIT_EVENT(EVENT) EVENT()

