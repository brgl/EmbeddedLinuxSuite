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
 * @file    unit_Events.cpp
 */

#include "ElsUnit.hpp"

#include <els/Events.hpp>

namespace {

int eventVal = 0;

class Listener
{
public:
    Listener(void) : ELS_INIT_LISTENER(fire) {}
    ELS_LISTENER(fire, Listener, doFire, int);
private:
    void doFire(int i) { eventVal = i; }
};

class Broadcaster
{
public:
    Broadcaster(void) : ELS_INIT_EVENT(broadcast) {}
    ELS_EVENT(broadcast, int, Broadcaster);
    void doBroadcast(int i) { this->broadcast(i); }
};

}

ELSUNIT_SIMPLE_TESTCASE(Events, basicEvent)
{
    Listener listener;
    Broadcaster broadcaster;

    broadcaster.broadcast += listener.fire;
    broadcaster.doBroadcast(2);
    ELSUNIT_ASSERT_EQ(2, eventVal);
}


