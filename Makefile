#################################################################################################
# Globals
#################################################################################################
CXXFLAGS =	-Wall -fPIC -O2 -D_GNU_SOURCE -fvisibility=hidden -Wno-psabi
LDFLAGS =	-Wl,-E -L./
DEBUGFLAGS =
LDSOFLAGS =	-shared -rdynamic
INCLUDEDIR =	-I./include
CROSS_COMPILE =
COMPILER =	$(CROSS_COMPILE)$(CXX)
LINKER =	$(CROSS_COMPILE)$(CXX)

#################################################################################################
# libels-common
#################################################################################################
LIBELS_COMMON_TARGET =	./libels-common.so
LIBELS_COMMON_SONAME =	libels-common.so
LIBELS_COMMON_OBJS =	./lib/Exception.o							\
			./lib/Mutex.o								\
			./lib/AutoMutex.o							\
			./lib/Condition.o							\
			./lib/Regex.o								\
			./lib/Utils.o								\
			./lib/System.o								\
			./lib/String.o								\
			./lib/OptParser.o							\
			./lib/ByteArray.o							\
			./lib/SoLoader.o							\
			./lib/Directory.o							\
			./lib/IThread.o								\
			./lib/INamedThread.o							\
			./lib/IRunnable.o							\
			./lib/ThreadPool.o							\
			./lib/Timeval.o								\
			./lib/Date.o								\
			./lib/File.o								\
			./lib/ILogHandler.o							\
			./lib/Logger.o								\
			./lib/TerminalLogHandler.o						\
			./lib/SyslogLogHandler.o						\
			./lib/FilePath.o							\
			./lib/ISocket.o								\
			./lib/UnixSocket.o							\
			./lib/TcpSocket.o							\
			./lib/IServer.o								\
			./lib/UnixServer.o							\
			./lib/TcpServer.o							\
			./lib/ConfigParser.o							\
			./lib/SockAddr.o							\
			./lib/AddrInfo.o							\
			./lib/SockHelpers.o							\
			./lib/SharedPtr.o							\
			./lib/ReadWriteLock.o
LIBELS_COMMON_LIBS =	-pthread -ldl

libels-common.so:	$(LIBELS_COMMON_OBJS)
	$(LINKER) -o $(LIBELS_COMMON_TARGET) $(LIBELS_COMMON_OBJS) $(LDFLAGS) $(LDSOFLAGS)	\
		$(DEBUGFLAGS) -Wl,-soname,$(LIBELS_COMMON_SONAME) $(LIBELS_COMMON_LIBS)

#################################################################################################
# all
#################################################################################################
all:		libels-common.so

#################################################################################################
# test
#################################################################################################
ELS_UNIT_TARGET = 	./els_unit
ELS_UNIT_OBJS =		./test/ElsUnit.o							\
			./test/unit_Exception.o							\
			./test/unit_Regex.o							\
			./test/unit_String.o							\
			./test/unit_ByteArray.o							\
			./test/unit_AtomicInt.o							\
			./test/unit_OptParser.o							\
			./test/unit_Utils.o							\
			./test/unit_IThread.o							\
			./test/unit_INamedThread.o						\
			./test/unit_ThreadPool.o						\
			./test/unit_Singleton.o							\
			./test/unit_Condition.o							\
			./test/unit_Timeval.o							\
			./test/unit_Date.o							\
			./test/unit_FilePath.o							\
			./test/unit_Mutex.o							\
			./test/unit_AutoMutex.o							\
			./test/unit_SoLoader.o							\
			./test/unit_System.o							\
			./test/unit_Directory.o							\
			./test/unit_Logger.o							\
			./test/unit_ConfigParser.o						\
			./test/unit_SockAddr.o							\
			./test/unit_SharedPtr.o							\
			./test/unit_Events.o
ELS_UNIT_LIBS =		-lgtest -pthread

test:		$(ELS_UNIT_OBJS) $(LIBELS_COMMON_OBJS) $(LIBELS_BUS_OBJS)
	$(LINKER) -o $(ELS_UNIT_TARGET) $(ELS_UNIT_OBJS) $(LIBELS_BUS_OBJS)			\
		$(LIBELS_COMMON_OBJS) $(LDFLAGS) $(DEBUGFLAGS) $(ELS_UNIT_LIBS)			\
		$(LIBELS_COMMON_LIBS)
	$(ELS_UNIT_TARGET)

#################################################################################################
# clean
#################################################################################################
clean:
	rm -f $(LIBELS_COMMON_OBJS)
	rm -f $(LIBELS_COMMON_TARGET)
	rm -f $(ELS_UNIT_OBJS)
	rm -f $(ELS_UNIT_TARGET)
	
#################################################################################################
# doc
#################################################################################################
DOXYPATH =	./doc/Doxyfile
DOXYDIR =	./doc/out

doc:
	doxygen $(DOXYPATH)

doc_clean:
	rm -rf $(DOXYDIR)

#################################################################################################
# other
#################################################################################################

.PRECIOUS:	%.cpp
.SUFFIXES:
.SUFFIXES:	.o .cpp
.PHONY:		all clean test doc doc_clean

.cpp.o:
	$(COMPILER) -c -o $*.o $(CXXFLAGS) $(INCLUDEDIR) $(DEBUGFLAGS) $*.cpp


