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
 * @file    ConfigParser.cpp
 */

#include <els/ConfigParser.hpp>
#include <els/File.hpp>
#include <els/String.hpp>
#include <els/Regex.hpp>

#include <cstdarg>
#include <cstring>
#include <cstdio>

ELS_BEGIN_NAMESPACE_2(els, misc)

ConfigParser::EntryMap::EntryMap(void)
    : _M_entries()
{

}

ConfigParser::EntryMap::EntryMap(const EntryMap& other)
    : _M_entries(other._M_entries)
{

}

ConfigParser::EntryMap&
ConfigParser::EntryMap::operator =(const EntryMap& other)
{
    this->_M_entries = other._M_entries;
    return *this;
}

ConfigParser::EntryMap::~EntryMap(void)
{

}

const std::string& ConfigParser::EntryMap::entry(const std::string& key) const
{
    _T_EntryMap::const_iterator it = this->_M_entries.find(key);
    if (it == this->_M_entries.end())
        throw except::KeyNotFound("Key not found: %s", key.c_str());
    return it->second;
}

bool ConfigParser::EntryMap::hasEntry(const std::string& key) const
{
    return (this->_M_entries.find(key) != this->_M_entries.end());
}

ConfigParser::SectionMap::SectionMap(void)
    : _M_sections()
{

}

ConfigParser::SectionMap::SectionMap(const SectionMap& other)
    : _M_sections(other._M_sections)
{

}

ConfigParser::SectionMap&
ConfigParser::SectionMap::operator =(const SectionMap& other)
{
    this->_M_sections = other._M_sections;
    return *this;
}

ConfigParser::SectionMap::~SectionMap(void)
{

}

const ConfigParser::EntryMap&
ConfigParser::SectionMap::sect(const std::string& key) const
{
    _T_SectionMap::const_iterator it = this->_M_sections.find(key);
    if (it == this->_M_sections.end())
        throw except::KeyNotFound("Key not found: %s", key.c_str());
    return it->second;
}

bool ConfigParser::SectionMap::hasSect(const std::string& key) const
{
    return (this->_M_sections.find(key) != this->_M_sections.end());
}

ElsSize ConfigParser::SectionMap::size(void) const
{
    return this->_M_sections.size();
}

ConfigParser::SectionMap::const_iterator
ConfigParser::SectionMap::begin(void) const
{
    return this->_M_sections.begin();
}

ConfigParser::SectionMap::const_iterator
ConfigParser::SectionMap::end(void) const
{
    return this->_M_sections.end();
}

ConfigParser::SectionMap::const_reverse_iterator
ConfigParser::SectionMap::rbegin(void) const
{
    return this->_M_sections.rbegin();
}

ConfigParser::SectionMap::const_reverse_iterator
ConfigParser::SectionMap::rend(void) const
{
    return this->_M_sections.rend();
}

ConfigParser::ConfigParser(void)
    : _M_globals(),
      _M_types()
{

}

ConfigParser::ConfigParser(const ConfigParser& other)
    : _M_globals(other._M_globals),
      _M_types(other._M_types)
{

}

ConfigParser& ConfigParser::operator =(const ConfigParser& other)
{
    this->_M_globals = other._M_globals;
    this->_M_types = other._M_types;
    return *this;
}

ConfigParser::~ConfigParser(void) throw()
{

}

void ConfigParser::parseFile(const std::string& path)
{
    fs::File in;
    StringList lines;

    in.open(path, fs::File::MODE_READ);
    while (!in.eof())
        lines.push_back(in.readline());
    this->_M_parse(lines);
}

void ConfigParser::parseStr(const std::string& str)
{
    StringList lines;

    lines = str::split(str, "\n");
    this->_M_parse(lines);
}

const ConfigParser::EntryMap& ConfigParser::globals(void) const
{
    return this->_M_globals;
}

const ConfigParser::SectionMap&
ConfigParser::type(const std::string& type) const
{
    _T_TypeMap::const_iterator it = this->_M_types.find(type);
    if (it == this->_M_types.end())
        throw except::KeyNotFound("Key not found: %s", type.c_str());
    return it->second;
}

bool ConfigParser::hasType(const std::string& type) const
{
    return (this->_M_types.find(type) != this->_M_types.end());
}

std::string ConfigParser::toStr(void) const
{
    std::string str;


    for (EntryMap::_T_EntryMap::const_iterator
            it = this->_M_globals._M_entries.begin();
            it != this->_M_globals._M_entries.end(); ++it)
    {
        str.append(str::buildString("%s = \"%s\"\n",
                it->first.c_str(), it->second.c_str()));
    }
    for (_T_TypeMap::const_iterator typeIt = this->_M_types.begin();
            typeIt != this->_M_types.end(); ++typeIt)
    {
        for (SectionMap::_T_SectionMap::const_iterator
                secIt = typeIt->second._M_sections.begin();
                secIt != typeIt->second._M_sections.end(); ++secIt)
        {
            str.append(str::buildString("[%s:%s]\n",
                    typeIt->first.c_str(),
                    secIt->first.c_str()));
            for (EntryMap::_T_EntryMap::const_iterator
                    entIt = secIt->second._M_entries.begin();
                    entIt != secIt->second._M_entries.end(); ++entIt)
            {
                str.append(str::buildString("%s = \"%s\"\n",
                        entIt->first.c_str(),
                        entIt->second.c_str()));
            }
        }
    }

    if ((!str.empty()) && (*str.rbegin()) == '\n')
        str.erase(str.rbegin().base()-1);

    return str;
}

void ConfigParser::_M_parse(const StringList& lines)
{
    static const Regex regEntry("[a-zA-Z]+[a-zA-Z0-9]*[\\ \\t]*\\=.*");
    static const Regex regSect(
            "\\[[\\ \\t]*[a-zA-Z]+[a-zA-Z0-9]*[\\ \\t]*\\:[\\ \\t]*"
            "[a-zA-Z]+[a-zA-Z0-9]*[\\ \\t]*\\][\\ \\t]*");

    bool atGlobals = true;
    unsigned lineno = 0;
    EntryMap newEntryMap;
    std::string curType;
    std::string curSect;

    try
    {
        for (StringList::const_iterator it = lines.begin();
                it != lines.end(); it++)
        {
            std::string line = *it;

            ++lineno;
            _S_stripComments(line);
            if (str::isBlank(line))
                continue;

            if (regEntry.match(line))
            {
                newEntryMap._M_entries.insert(_S_splitEntry(line));
            }
            else if (regSect.match(line))
            {
                if (atGlobals)
                {
                    this->_M_globals = newEntryMap;
                    atGlobals = false;
                    newEntryMap._M_entries.clear();
                }
                else
                {
                    _S_endSection(this->_M_types, newEntryMap,
                            curType, curSect, lineno);
                }

                StringList ts = str::split(line, "[]: \t");
                if (ts.size() != 2)
                {
                    _S_throwSyntaxError(lineno,
                            "Invalid section definition");
                }
                curType = ts.at(0);
                curSect = ts.at(1);
            }
            else
            {
                _S_throwSyntaxError(lineno,
                        "Invalid line format");
            }
        }

        _S_endSection(this->_M_types, newEntryMap,
                curType, curSect, lineno);
    }
    catch (const except::InvalidArgument& e)
    {
        _S_throwSyntaxError(lineno, e.what());
    }
}

void ConfigParser::_S_endSection(_T_TypeMap& types, EntryMap& entr,
        std::string& curType, std::string& curSect, unsigned lineno)
{
    if (entr._M_entries.empty())
    {
        _S_throwSyntaxError(lineno, "Empty section");
    }

    types[curType]._M_sections.insert(
            std::make_pair(curSect, entr));
    curSect.clear();
    curType.clear();
    entr._M_entries.clear();
}

ConfigParser::_T_Entry ConfigParser::_S_splitEntry(const std::string& line)
{
    size_t pos = 0;
    _T_Entry entr;

    pos = line.find('=');
    entr.first = str::strip(line.substr(0, pos));
    entr.second = _S_handleParens(str::strip(line.substr(pos+1)));

    return entr;
}

void ConfigParser::_S_stripComments(std::string& line)
{
    size_t pos = line.find('#');
    if (pos != std::string::npos)
        line.erase(pos);
}

std::string ConfigParser::_S_handleParens(const std::string& str)
{
    std::string ret(str);

    if (ret.size() > 1)
    {
        if (((*ret.begin()) == '\"') && ((*ret.rbegin()) == '\"'))
        {
            if (ret.size() == 2)
            {
                throw except::InvalidArgument(
                        "Stripping will result in an empty string");
            }

            ret.erase(ret.begin());
            ret.erase(ret.rbegin().base()-1);
        }
    }

    return ret;
}

void ConfigParser::_S_throwSyntaxError(unsigned lineno, const char* fmt, ...)
{
    ::va_list va;
    char buf[512];

    ::memset(buf, 0, sizeof(buf));
    ::va_start(va, fmt);
    ::snprintf(buf, sizeof(buf), fmt, va);
    ::va_end(va);

    throw SyntaxError("Syntax error at line %d: %s", lineno, buf);
}

ELS_DEFINE_NESTED_EXCEPTION(SyntaxError, ConfigParser, except::Exception);

ELS_END_NAMESPACE_2

