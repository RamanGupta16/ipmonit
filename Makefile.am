AUTOMAKE_OPTIONS = foreign

CXXFLAGS = -Wall -Wshadow -Wcast-qual

bin_PROGRAMS = ipmonit
ipmonit_SOURCES = \
	common/SystemUtils.cpp \
	src/NetLinkSocket.cpp \
	src/NetLinkListenClient.cpp \
	src/SocketInterface.cpp \
	binary/ipmonit.cpp
