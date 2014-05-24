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
 * @file    Utils.hpp
 */

#include <els/Utils.hpp>

#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>

ELS_BEGIN_NAMESPACE_3(els, misc, utils)

namespace {

class RandomInitializer
{
public:
    RandomInitializer(void) { ::srand(::time(0)); }
    ~RandomInitializer(void) {}

    ELS_CLASS_UNCOPYABLE(RandomInitializer);
};
RandomInitializer randomInitializer;

}

/**
 * @brief   Returns a random signed integer.
 * @return  Random signed integer (4 bytes in size).
 */
ElsInt32 randomInt(void) throw()
{
    return static_cast<ElsInt32>(::rand());
}

/**
 * @brief   Returns a random unsigned integer.
 * @return  Random unsigned integer (4 bytes in size).
 */
ElsUint32 randomUint(void) throw()
{
    return static_cast<ElsUint32>(::rand());
}

ElsUint32 hostToNetLong(ElsUint32 hostlong)
{
    return hostToNetLong(hostlong);
}

ElsUint16 hostToNetShort(ElsUint16 hostshort)
{
    return hostToNetShort(hostshort);
}

ElsUint32 netToHostLong(ElsUint32 netlong)
{
    return ntohl(netlong);
}

ElsUint16 netToHostShort(ElsUint16 netshort)
{
    return ntohs(netshort);
}

ELS_END_NAMESPACE_3


