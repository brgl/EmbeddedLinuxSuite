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
 * @file    ThreadPool.cpp
 */

#include <els/ThreadPool.hpp>
#include <els/Exception.hpp>
#include <els/AutoMutex.hpp>

ELS_BEGIN_NAMESPACE_2(els, thread)

const size_t ThreadPool::DEF_NUM_THREADS = 16;

ThreadPool::ThreadPool(void)
    : _M_tasks(),
      _M_taskMutex(),
      _M_jobs(),
      _M_jobMutex(),
      _M_cond()
{

}

ThreadPool::~ThreadPool(void)
{
    this->_M_clearJobs();
}

void ThreadPool::schedule(IRunnable* task, bool autoDelete)
{
    this->_M_taskMutex.lock();
    this->_M_tasks.push_back(std::make_pair(task, autoDelete));
    this->_M_taskMutex.unlock();
    this->_M_cond.unblockOne();
}

void ThreadPool::start(size_t numJobs)
{
    _T_Job* newJob = 0;

    AutoMutex am(this->_M_jobMutex);

    if (!this->_M_jobs.empty())
        except::throwLogicError("ThreadPool already active");

    this->_M_jobs.resize(numJobs, 0);
    for (unsigned i = 0; i < numJobs; ++i)
    {
        newJob = new _T_Job(this);
        newJob->start();
        this->_M_jobs.at(i) = newJob;
    }
}

void ThreadPool::stop(void)
{
    AutoMutex am(this->_M_jobMutex);

    for (_T_JobList::iterator it = this->_M_jobs.begin();
            it != this->_M_jobs.end(); ++it)
        (*it)->stop();
    this->_M_cond.unblockAll();

    for (_T_JobList::iterator it = this->_M_jobs.begin();
            it != this->_M_jobs.end(); ++it)
        (*it)->join();

    this->_M_clearJobs();
}

void ThreadPool::_M_clearJobs(void)
{
    for (_T_JobList::iterator it = this->_M_jobs.begin();
            it != this->_M_jobs.end(); ++it)
        delete *it;
    this->_M_jobs.clear();
}

ThreadPool::_T_Job::_T_Job(ThreadPool* owner)
    : IThread(),
      _M_owner(owner)
{

}

ThreadPool::_T_Job::~_T_Job(void)
{

}

int ThreadPool::_T_Job::_M_run(void)
{
    while (!this->_M_stopRequested())
    {
        this->_M_owner->_M_taskMutex.lock();
        if (this->_M_owner->_M_tasks.empty())
        {
            this->_M_owner->_M_taskMutex.unlock();
            this->_M_owner->_M_cond.block();
        }
        else
        {
            IRunnable* task = this->_M_owner->_M_tasks.front().first;
            bool autoDelete = this->_M_owner->_M_tasks.front().second;
            this->_M_owner->_M_tasks.pop_front();
            this->_M_owner->_M_taskMutex.unlock();
            task->run();
            if (autoDelete)
                delete task;
        }
    }

    return 0;
}

ELS_END_NAMESPACE_2


