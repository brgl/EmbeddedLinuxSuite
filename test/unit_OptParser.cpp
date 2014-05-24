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
 * @file    unit_OptParser.cpp
 */

#include "ElsUnit.hpp"

#include <els/OptParser.hpp>
#include <els/StringList.hpp>
#include <els/Exception.hpp>

ELSUNIT_SIMPLE_TESTCASE(OptParser, noArgsSet)
{
    static const char* args[] = {"dummy1", "dummy2"};

    els::misc::OptParser op;
    ELSUNIT_EXPECT_NO_THROW(op.parse(2, args));
    ELSUNIT_ASSERT_EQ(2, op.getNonopts().size());
    ELSUNIT_EXPECT_STRING_EQ(op.getNonopts().at(0), "dummy1");
    ELSUNIT_EXPECT_STRING_EQ(op.getNonopts().at(1), "dummy2");
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, emptyArgList)
{
    els::misc::OptParser op;
    op.addOpt('d', "dummy", els::misc::OptParser::OPT_NOARG);
    ELSUNIT_EXPECT_NO_THROW(op.parse(els::misc::StringList()));
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, optionThrow)
{
    static const char* args[] = {"dummy", "-d"};

    els::misc::OptParser op;
    op.addOpt('d', els::misc::OptParser::OPT_NOARG,
            els::misc::OptParser::OPT_THROW);
    ELSUNIT_EXPECT_EXCEPTION(op.parse(2, args),
            els::misc::OptParser::OptThrowable);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, invalidShortOpt)
{
    els::misc::OptParser op;

    ELSUNIT_EXPECT_EXCEPTION(op.addOpt('-', els::misc::OptParser::OPT_NOARG),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_NO_THROW(op.addOpt('r', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_EXPECT_EXCEPTION(op.addOpt('3', els::misc::OptParser::OPT_NOARG),
            els::except::InvalidArgument);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, invalidLongOpt)
{
    els::misc::OptParser op;

    ELSUNIT_EXPECT_EXCEPTION(op.addOpt("3453425",
            els::misc::OptParser::OPT_NOARG),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_NO_THROW(op.addOpt("longopt",
            els::misc::OptParser::OPT_NOARG));
    ELSUNIT_EXPECT_NO_THROW(op.addOpt("long-opt",
            els::misc::OptParser::OPT_NOARG));
    ELSUNIT_EXPECT_EXCEPTION(op.addOpt("4long-opt",
            els::misc::OptParser::OPT_NOARG),
            els::except::InvalidArgument);
    ELSUNIT_EXPECT_EXCEPTION(op.addOpt("dfggg&^%%$%fdgfd",
            els::misc::OptParser::OPT_NOARG),
            els::except::InvalidArgument);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, undefinedShortOption)
{
    static const char* args[] = {"dummy", "-d", "-t"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('d', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_EXPECT_EXCEPTION(op.parse(3, args),
            els::misc::OptParser::ParsingError);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, noSuchOptionShort)
{
    static const char* args[] = {"dummy", "-d", "-t"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('d', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('t', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.parse(3, args));
    ELSUNIT_EXPECT_EXCEPTION(op.getOpt('a'),
            els::misc::OptParser::NoSuchOption);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, noSuchOptionLong)
{
    static const char* args[] = {"dummy", "--dd", "--tt"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt("dd", els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt("tt", els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.parse(3, args));
    ELSUNIT_EXPECT_EXCEPTION(op.getOpt("aa"),
            els::misc::OptParser::NoSuchOption);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, notParsedYet)
{
    els::misc::OptParser op;
    ELSUNIT_EXPECT_EXCEPTION(op.getOpt('d'), els::except::LogicError);
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argReqShort)
{
    static const char* argsBad[] = {"dummy", "-d"};
    static const char* argsGood[] = {"dummy", "-d", "dummyArg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_EXPECT_EXCEPTION(op.parse(2, argsBad),
                els::misc::OptParser::ParsingError);
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_ASSERT_NO_THROW(op.parse(3, argsGood));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').set());
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("dummyArg", op.getOpt('d').getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argReqLong)
{
    static const char* argsBad[] = {"dummy", "--somearg"};
    static const char* argsGood[] = {"dummy", "--somearg", "dummyArg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_EXPECT_EXCEPTION(op.parse(2, argsBad),
                els::misc::OptParser::ParsingError);
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_ASSERT_NO_THROW(op.parse(3, argsGood));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt("somearg"));
        ELSUNIT_ASSERT_TRUE(op.getOpt("somearg").set());
        ELSUNIT_ASSERT_TRUE(op.getOpt("somearg").hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("dummyArg", op.getOpt("somearg").getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argReqLongEq)
{
    static const char* argsBad1[] = {"dummy", "--somearg"};
    static const char* argsBad2[] = {"dummy", "--somearg="};
    static const char* argsGood[] = {"dummy", "--somearg=dummyArg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_EXPECT_EXCEPTION(op.parse(2, argsBad1),
                els::misc::OptParser::ParsingError);
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_EXPECT_EXCEPTION(op.parse(2, argsBad2),
                els::misc::OptParser::ParsingError);
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, argsGood));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt("somearg"));
        ELSUNIT_ASSERT_TRUE(op.getOpt("somearg").set());
        ELSUNIT_ASSERT_TRUE(op.getOpt("somearg").hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("dummyArg", op.getOpt("somearg").getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argReqShortJoined)
{
    static const char* argsBad[] = {"dummy", "-d"};
    static const char* argsGood[] = {"dummy", "-dsomearg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_EXPECT_EXCEPTION(op.parse(2, argsBad),
                els::misc::OptParser::ParsingError);
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGREQ));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, argsGood));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').set());
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("somearg", op.getOpt('d').getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, multipleArgsOptShort)
{
    static const char* args[] = {
            "dummy",
            "-d", "dummyArg1",
            "-d", "dummyArg2",
            "-d", "dummyArg3"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
            els::misc::OptParser::OPT_ARGREQ));
    ELSUNIT_ASSERT_NO_THROW(op.parse(7, args));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
    ELSUNIT_EXPECT_TRUE(op.getOpt('d').set());
    ELSUNIT_ASSERT_TRUE(op.getOpt('d').hasArgs());
    ELSUNIT_ASSERT_EQ(3, op.getOpt('d').getArgs().size());
    ELSUNIT_EXPECT_STRING_EQ("dummyArg1", op.getOpt('d').getArgs().at(0));
    ELSUNIT_EXPECT_STRING_EQ("dummyArg2", op.getOpt('d').getArgs().at(1));
    ELSUNIT_EXPECT_STRING_EQ("dummyArg3", op.getOpt('d').getArgs().at(2));
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, multipleArgsOptLong)
{
    static const char* args[] = {
            "dummy",
            "--someopt", "dummyArg1",
            "--someopt", "dummyArg2",
            "--someopt", "dummyArg3"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt("someopt",
            els::misc::OptParser::OPT_ARGREQ));
    ELSUNIT_ASSERT_NO_THROW(op.parse(7, args));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt("someopt"));
    ELSUNIT_EXPECT_TRUE(op.getOpt("someopt").set());
    ELSUNIT_ASSERT_TRUE(op.getOpt("someopt").hasArgs());
    ELSUNIT_ASSERT_EQ(3, op.getOpt("someopt").getArgs().size());
    ELSUNIT_EXPECT_STRING_EQ("dummyArg1",
            op.getOpt("someopt").getArgs().at(0));
    ELSUNIT_EXPECT_STRING_EQ("dummyArg2",
            op.getOpt("someopt").getArgs().at(1));
    ELSUNIT_EXPECT_STRING_EQ("dummyArg3",
            op.getOpt("someopt").getArgs().at(2));
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argOptShort)
{
    static const char* args1[] = {"dummy", "-d"};
    static const char* args2[] = {"dummy", "-d", "dummyArg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, args1));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
        ELSUNIT_EXPECT_TRUE(op.getOpt('d').set());
        ELSUNIT_EXPECT_FALSE(op.getOpt('d').hasArgs());
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(3, args2));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').set());
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("dummyArg", op.getOpt('d').getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argOptLong)
{
    static const char* args1[] = {"dummy", "--someopt"};
    static const char* args2[] = {"dummy", "--someopt", "dummyArg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("someopt",
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, args1));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt("someopt"));
        ELSUNIT_EXPECT_TRUE(op.getOpt("someopt").set());
        ELSUNIT_EXPECT_FALSE(op.getOpt("someopt").hasArgs());
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("someopt",
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(3, args2));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt("someopt"));
        ELSUNIT_ASSERT_TRUE(op.getOpt("someopt").set());
        ELSUNIT_ASSERT_TRUE(op.getOpt("someopt").hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("dummyArg", op.getOpt("someopt").getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argOptLongEq)
{
    static const char* argsGood1[] = {"dummy", "--somearg"};
    static const char* argsBad[] = {"dummy", "--somearg="};
    static const char* argsGood2[] = {"dummy", "--somearg=dummyArg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, argsGood1));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt("somearg"));
        ELSUNIT_EXPECT_TRUE(op.getOpt("somearg").set());
        ELSUNIT_EXPECT_FALSE(op.getOpt("somearg").hasArgs());
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_EXPECT_EXCEPTION(op.parse(2, argsBad),
                els::misc::OptParser::ParsingError);
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt("somearg",
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, argsGood2));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt("somearg"));
        ELSUNIT_ASSERT_TRUE(op.getOpt("somearg").set());
        ELSUNIT_ASSERT_TRUE(op.getOpt("somearg").hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("dummyArg", op.getOpt("somearg").getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, argOptShortJoined)
{
    static const char* args1[] = {"dummy", "-d"};
    static const char* args2[] = {"dummy", "-dsomearg"};

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, args1));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
        ELSUNIT_EXPECT_TRUE(op.getOpt('d').set());
        ELSUNIT_EXPECT_FALSE(op.getOpt('d').hasArgs());
    }

    {
        els::misc::OptParser op;
        ELSUNIT_ASSERT_NO_THROW(op.addOpt('d',
                els::misc::OptParser::OPT_ARGOPT));
        ELSUNIT_ASSERT_NO_THROW(op.parse(2, args2));
        ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').set());
        ELSUNIT_ASSERT_TRUE(op.getOpt('d').hasArgs());
        ELSUNIT_EXPECT_STRING_EQ("somearg", op.getOpt('d').getArg());
    }
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, shortOptsJoinedGood)
{
    static const char* args[] = {"dummy", "-abcd"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('a', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('b', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('c', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('d', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('e', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.parse(2, args));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt('a'));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt('b'));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt('c'));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt('d'));
    ELSUNIT_ASSERT_NO_THROW(op.getOpt('e'));
    ELSUNIT_EXPECT_TRUE(op.getOpt('a').set());
    ELSUNIT_EXPECT_TRUE(op.getOpt('b').set());
    ELSUNIT_EXPECT_TRUE(op.getOpt('c').set());
    ELSUNIT_EXPECT_TRUE(op.getOpt('d').set());
    ELSUNIT_EXPECT_FALSE(op.getOpt('e').set());
}

ELSUNIT_SIMPLE_TESTCASE(OptParser, shortOptsJoinedBad)
{
    static const char* args[] = {"dummy", "-abcfe"};

    els::misc::OptParser op;
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('a', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('b', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('c', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('d', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_ASSERT_NO_THROW(op.addOpt('e', els::misc::OptParser::OPT_NOARG));
    ELSUNIT_EXPECT_EXCEPTION(op.parse(2, args),
            els::misc::OptParser::ParsingError);
}

