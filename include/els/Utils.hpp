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
 * @brief   Various utility functions.
 */

#pragma once

#include "Macros.hpp"
#include "Types.hpp"

ELS_BEGIN_NAMESPACE_3(els, misc, utils)

ELS_EXPORT_SYMBOL ElsInt32 randomInt(void) throw();
ELS_EXPORT_SYMBOL ElsUint32 randomUint(void) throw();

ELS_EXPORT_SYMBOL ElsUint32 hostToNetLong(ElsUint32 hostlong);
ELS_EXPORT_SYMBOL ElsUint16 hostToNetShort(ElsUint16 hostshort);
ELS_EXPORT_SYMBOL ElsUint32 netToHostLong(ElsUint32 netlong);
ELS_EXPORT_SYMBOL ElsUint16 netToHostShort(ElsUint16 netshort);

ELS_END_NAMESPACE_3

