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
 * @file    IThread.hpp
 */

#pragma once

#include "Macros.hpp"
#include "Mutex.hpp"
#include "Condition.hpp"
#include "Exception.hpp"
#include "Atomic.hpp"

#include <pthread.h>
#include <sys/types.h>
#include <memory>

ELS_BEGIN_NAMESPACE_2(els, thread)

class IThread
{
public:

    ELS_DECLARE_NESTED_EXCEPTION(ThreadError, except::Exception);

    enum ThreadState
    {
        THREAD_INITIALIZED = 0,
        THREAD_STARTING,
        THREAD_RUNNING,
        THREAD_SLEEPING,
        THREAD_FINISHED,
        THREAD_CANCELED,
    };

    class ThreadRetval
    {
    public:

        ELS_EXPORT_SYMBOL explicit ThreadRetval(int retval,
                const std::string& error = std::string())
            : _M_retval(retval),
              _M_error(error)
        {

        }

        ELS_EXPORT_SYMBOL int retval(void) const
            { return this->_M_retval; }
        ELS_EXPORT_SYMBOL bool hasError(void) const
            { return !this->_M_error.empty(); }
        ELS_EXPORT_SYMBOL const std::string& errMsg(void) const
            { return this->_M_error; }

    private:

        int _M_retval;
        std::string _M_error;
    };

    ELS_EXPORT_SYMBOL IThread(void);
    ELS_EXPORT_SYMBOL virtual ~IThread(void);

    ELS_EXPORT_SYMBOL ThreadState state(void) const;
    ELS_EXPORT_SYMBOL void start(void);
    ELS_EXPORT_SYMBOL void stop(void);
    ELS_EXPORT_SYMBOL void requestStop(void);
    ELS_EXPORT_SYMBOL void cancel(void);
    ELS_EXPORT_SYMBOL void join(void);
    ELS_EXPORT_SYMBOL bool tryjoin(void);
    ELS_EXPORT_SYMBOL void reset(void);
    ELS_EXPORT_SYMBOL void wake(void);
    ELS_EXPORT_SYMBOL ::pid_t tid(void) const;
    ELS_EXPORT_SYMBOL const ThreadRetval& retval(void) const;

    ELS_EXPORT_SYMBOL static unsigned threadCount(void);

protected:

    virtual int _M_run(void) = 0;

    ELS_EXPORT_SYMBOL void _M_sleep(void);
    ELS_EXPORT_SYMBOL bool _M_stopRequested(void) const;

private:

    typedef std::auto_ptr<ThreadRetval> _T_RetvalPtr;

    ::pthread_t _M_id;
    ::pid_t _M_tid;
    mutable Mutex _M_mutex;
    Condition _M_cond;
    ThreadState _M_state;
    volatile bool _M_stopRequest;
    _T_RetvalPtr _M_retval;

    void _M_setState(ThreadState state);

    static void* _S_initThread(void* threadPtr);

    static AtomicInt _S_threadCount;
    static void _S_incThreadCount(void);
    static void _S_decThreadCount(void);

    ELS_CLASS_UNCOPYABLE(IThread);
};

ELS_END_NAMESPACE_2

