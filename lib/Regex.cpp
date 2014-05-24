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
 * @file    Regex.cpp
 */

#include <els/Regex.hpp>

#include <cstring>

ELS_BEGIN_NAMESPACE_2(els, misc)

const size_t Regex::_S_ERRBUF_SIZE;
const size_t Regex::_S_DEF_NUM_MATCHES;

/**
 * @brief   Default constructor. Initializes the internal structures.
 *
 * It doesn't compile any regular expression, so the object is not
 * ready to use.
 */
Regex::Regex(void)
    : _M_pattern(),
      _M_regex(),
      _M_compiled(false)
{

}

/**
 * @brief   This constructor compiles given regular expression pattern
 *          upon construction.
 * @param   pattern         Regular expression to be compiled.
 * @throw   RegexError      Error during compilation.
 */
Regex::Regex(const std::string& pattern)
    : _M_pattern(pattern),
      _M_regex(),
      _M_compiled(false)
{
    this->compile();
}

/**
 * @brief   Copy constructor. It only copies the pattern and compiles it
 *          on its own.
 * @param   other           Other instance of the Regex class.
 * @throw   RegexError      Error during compilation.
 */
Regex::Regex(const Regex& other)
    : _M_pattern(other._M_pattern),
      _M_regex(),
      _M_compiled(false)
{
    this->compile();
}

/**
 * @brief   Assignment operator. It only copies the pattern
 *          and compiles it on its own.
 * @param   other           Other instance of the Regex class.
 * @return  Reference to this object.
 * @throw   RegexError      Error during compilation.
 */
Regex& Regex::operator =(const Regex& other)
{
    this->reset();
    this->_M_pattern = other._M_pattern;
    this->compile();
    return *this;
}

/**
 * @brief   Destructor.
 */
Regex::~Regex(void) throw()
{
    if (this->_M_compiled)
        ::regfree(&_M_regex);
}

/**
 * @brief   Compiles given regular expression pattern.
 * @param   pattern         Regular expression to be compiled.
 * @throw   RegexError      Error during compilation.
 */
void Regex::compile(const std::string& pattern)
{
    this->_M_pattern = pattern;
    this->compile();
}

/**
 * @brief   Compiles the pattern previously set by the constructor
 *          or overloaded compile() method.
 * @throw   RegexError      Error during compilation.
 */
void Regex::compile(void)
{
    int retval = 0;
    char errbuf[_S_ERRBUF_SIZE];
    int flags = REG_EXTENDED | REG_NEWLINE;

    if (this->_M_compiled)
    {
        ::regfree(&this->_M_regex);
        this->_M_compiled = false;
    }

    retval = ::regcomp(&this->_M_regex, this->_M_pattern.c_str(), flags);
    if (retval != 0)
    {
        this->_M_extractError(retval, errbuf, _S_ERRBUF_SIZE);
        ::regfree(&this->_M_regex);
        ::memset(&this->_M_regex, 0, sizeof(::regex_t));
        throw Regex::RegexError("Error compiling regex %s: %s",
                this->_M_pattern.c_str(), errbuf);
    }

    this->_M_compiled = true;
}

/**
 * @brief   Matches given string against compiled regex pattern.
 * @param   str             String to be matched.
 * @return  True if string str matches the pattern, otherwise false.
 * @throw   RegexError      If an error during matching occured.
 * @throw   logic_error     If no pattern has been compiled yet.
 */
bool Regex::match(const std::string& str) const
{
    bool ret = false;
    int retval = 0;
    char errbuf[_S_ERRBUF_SIZE];

    this->_M_checkIfCompiled();

    retval = ::regexec(&this->_M_regex, str.c_str(), 0, 0, 0);
    if (retval == REG_NOERROR)
        ret = true;
    else if (retval == REG_NOMATCH)
        ret = false;
    else
    {
        this->_M_extractError(retval, errbuf, _S_ERRBUF_SIZE);
        throw Regex::RegexError(
                "Error matching \'%s\' to pattern \'%s\': %s",
                str.c_str(), this->_M_pattern.c_str(), errbuf);
    }

    return ret;
}

/**
 * @brief   Finds all matching substrings in given string.
 * @param   str
 * @param   maxMatches
 * @return  List of all matching strings.
 * @throw   RegexError      If an error during matching occured.
 */
StringList Regex::findMatches(const std::string& str, size_t maxMatches) const
{
    int retval = 0;
    char errbuf[_S_ERRBUF_SIZE];
    StringList retList;
    ::regmatch_t match;
    unsigned matchOffset = 0;

    this->_M_checkIfCompiled();
    do
    {
        ::memset(&match, 0, sizeof(::regmatch_t));
        retval = ::regexec(&this->_M_regex, str.c_str()+matchOffset,
                1, &match, 0);
        if (retval == REG_NOMATCH)
        {
            break;
        }
        else if (retval == REG_NOERROR)
        {
            retList.push_back(std::string(
                    (str.c_str()+matchOffset)+match.rm_so,
                    match.rm_eo-match.rm_so));
            matchOffset += match.rm_so+(*retList.rbegin()).size();
        }
        else
        {
            this->_M_extractError(retval, errbuf, _S_ERRBUF_SIZE);
            throw Regex::RegexError("Error searching for matches in \'%s\' "
                    "against pattern \'%s\': %s",
                    str.c_str(), this->_M_pattern.c_str(), errbuf);
        }
    }
    while (true);

    return retList;
}

/**
 * @brief   Discards the internal regex structures.
 */
void Regex::reset(void) throw()
{
    if (this->_M_compiled)
        ::regfree(&_M_regex);
    this->_M_compiled = false;
    ::memset(&this->_M_regex, 0, sizeof(::regex_t));
    this->_M_pattern.clear();
}

/**
 * @brief   Set internal pattern to given string.
 * @param   pattern     New pattern
 */
void Regex::setPattern(const std::string& pattern)
{
    this->_M_pattern = pattern;
}

/**
 * @brief   Getter for the internal regex pattern.
 * @return  Const reference to the internal pattern string.
 */
const std::string& Regex::getPattern(void) const throw()
{
    return this->_M_pattern;
}

void Regex::_M_checkIfCompiled(void) const
{
    if (!this->_M_compiled)
        except::throwLogicError("No pattern compiled");
}

void Regex::_M_extractError(int retval, char* buf, size_t bufsize) const
{
    ::memset(buf, 0, bufsize);
    ::regerror(retval, &this->_M_regex, buf, bufsize);
}

ELS_DEFINE_NESTED_EXCEPTION(RegexError, Regex, except::Exception);

ELS_END_NAMESPACE_2


