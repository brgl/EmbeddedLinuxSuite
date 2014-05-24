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
 * @file    ThreadPool.hpp
 */

#pragma once

#include "Macros.hpp"
#include "IRunnable.hpp"
#include "IThread.hpp"

#include <list>
#include <vector>
#include <utility>

ELS_BEGIN_NAMESPACE_2(els, thread)

class ThreadPool
{
public:

    ELS_EXPORT_SYMBOL static const size_t DEF_NUM_THREADS;

    ELS_EXPORT_SYMBOL ThreadPool(void);
    ELS_EXPORT_SYMBOL virtual ~ThreadPool(void);

    ELS_EXPORT_SYMBOL void schedule(IRunnable* task, bool autoDelete = false);
    ELS_EXPORT_SYMBOL void start(size_t numJobs = DEF_NUM_THREADS);
    ELS_EXPORT_SYMBOL void stop(void);

private:

    class _T_Job : public IThread
    {
    public:
        explicit _T_Job(ThreadPool* owner);
        virtual ~_T_Job(void);
    protected:
        virtual int _M_run(void);
    private:
        ThreadPool* _M_owner;
        ELS_CLASS_UNCOPYABLE(_T_Job);
    };

    typedef std::list<std::pair<IRunnable*, bool /* delete */ > > _T_TaskList;
    typedef std::vector<_T_Job*> _T_JobList;

    _T_TaskList _M_tasks;
    mutable Mutex _M_taskMutex;
    _T_JobList _M_jobs;
    mutable Mutex _M_jobMutex;
    Condition _M_cond;

    void _M_clearJobs(void);

    friend class _T_Job;

    ELS_CLASS_UNCOPYABLE(ThreadPool);
};

ELS_END_NAMESPACE_2

