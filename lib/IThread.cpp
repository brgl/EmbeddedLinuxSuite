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
 * @file    IThread.cpp
 */

#include <els/IThread.hpp>
#include <els/AutoMutex.hpp>
#include <els/System.hpp>

#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, thread)

IThread::IThread(void)
    : _M_id(),
      _M_tid(-1),
      _M_mutex(),
      _M_cond(),
      _M_state(THREAD_INITIALIZED),
      _M_stopRequest(false),
      _M_retval(0)
{

}

IThread::~IThread(void)
{

}

void* IThread::_S_initThread(void* threadPtr)
{
    int ret = 0;
    ThreadRetval* trv = 0;

    _S_incThreadCount();

    IThread *thisThread = static_cast<IThread*>(threadPtr);
    thisThread->_M_mutex.lock();
    thisThread->_M_tid = sys::getTid();
    thisThread->_M_state = THREAD_RUNNING;
    thisThread->_M_mutex.unlock();

    try
    {
        ret = thisThread->_M_run();
    }
    catch (const except::Exception& e)
    {
        trv = new IThread::ThreadRetval(-1, e.what());
        goto out;
    }

    trv = new IThread::ThreadRetval(ret);

out:
    thisThread->_M_setState(THREAD_FINISHED);
    _S_decThreadCount();
    return static_cast<void*>(trv);
}

IThread::ThreadState IThread::state(void) const
{
    AutoMutex am(this->_M_mutex);
    return this->_M_state;
}

void IThread::start(void)
{
    AutoMutex am(this->_M_mutex);

    if (this->_M_state != THREAD_INITIALIZED)
        except::throwLogicError("Thread not in INITIALIZED state");
    this->_M_state = THREAD_STARTING;

    int ret = ::pthread_create(&this->_M_id, 0, IThread::_S_initThread,
            static_cast<void*>(this));
    if (ret != 0)
    {
        throw ThreadError("Error creating new thread: %s",
                except::getErrnoStr(ret).c_str());
    }
}

void IThread::stop(void)
{
    this->_M_mutex.lock();
    if ((this->_M_state == THREAD_INITIALIZED)
            || (this->_M_state == THREAD_CANCELED))
    {
        this->_M_mutex.unlock();
        except::throwLogicError("Thread not running");
    }
    this->_M_stopRequest = true;
    this->_M_mutex.unlock();
    this->wake();
}


void IThread::requestStop(void)
{
    AutoMutex am(this->_M_mutex);

    if ((this->_M_state != THREAD_RUNNING)
            && (this->_M_state != THREAD_SLEEPING))
    {
        except::throwLogicError("Thread not alive");
    }

    this->_M_stopRequest = true;
}

void IThread::cancel(void)
{
    ThreadState state = this->state();

    if ((state != THREAD_RUNNING) && (state != THREAD_SLEEPING))
        except::throwLogicError("Thread not alive");

    int ret = ::pthread_cancel(this->_M_id);
    if (ret != 0)
    {
        throw ThreadError(
                "Error in pthread_cancel: %s",
                except::getErrnoStr(ret).c_str());
    }
    this->_M_setState(THREAD_CANCELED);
}

void IThread::join(void)
{
    int retval = 0;
    void* retptr = 0;

    retval = ::pthread_join(this->_M_id, &retptr);
    if (retval != 0)
    {
        throw ThreadError(
                "Error joining threads: %s",
                except::getErrnoStr(retval).c_str());
    }

    this->_M_mutex.lock();
    this->_M_retval.reset(
            this->_M_state == THREAD_CANCELED ? 0
            : static_cast<IThread::ThreadRetval*>(retptr));
    this->_M_mutex.unlock();
}

bool IThread::tryjoin(void)
{
    this->_M_mutex.lock();
    if ((this->_M_state != THREAD_FINISHED)
            && (this->_M_state != THREAD_CANCELED))
    {
        this->_M_mutex.unlock();
        return false;
    }
    this->_M_mutex.unlock();

    this->join();
    return true;
}

void IThread::reset(void)
{
    ThreadState state = this->state();

    if ((state == THREAD_RUNNING)
            || (state == THREAD_SLEEPING)
            || (state == THREAD_STARTING))
    {
        except::throwLogicError("Thread not stopped");
    }

    ::memset(&this->_M_id, 0, sizeof(::pthread_t));
    this->_M_tid = -1;
    this->_M_state = THREAD_INITIALIZED;
    this->_M_stopRequest = false;
    this->_M_retval.reset();
}

void IThread::wake(void)
{
    if (this->state() == THREAD_SLEEPING)
        this->_M_cond.unblockAll();
}

::pid_t IThread::tid(void) const
{
    AutoMutex am(this->_M_mutex);
    return this->_M_tid;
}

const IThread::ThreadRetval& IThread::retval(void) const
{
    AutoMutex am(this->_M_mutex);

    if (this->_M_state != THREAD_FINISHED)
        except::throwLogicError("Thread not stopped");
    return *(this->_M_retval);
}

void IThread::_M_sleep(void)
{
    this->_M_setState(THREAD_SLEEPING);
    this->_M_cond.block();
    this->_M_setState(THREAD_RUNNING);
}

bool IThread::_M_stopRequested(void) const
{
    AutoMutex am(this->_M_mutex);
    return this->_M_stopRequest;
}

void IThread::_M_setState(ThreadState state)
{
    this->_M_mutex.lock();
    this->_M_state = state;
    this->_M_mutex.unlock();
}

AtomicInt IThread::_S_threadCount(0);

void IThread::_S_incThreadCount(void)
{
    _S_threadCount.inc();
}

void IThread::_S_decThreadCount(void)
{
    _S_threadCount.dec();
}

unsigned IThread::threadCount(void)
{
    return _S_threadCount.get();
}

ELS_DEFINE_NESTED_EXCEPTION(ThreadError, IThread, except::Exception);

ELS_END_NAMESPACE_2


