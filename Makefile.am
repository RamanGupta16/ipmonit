AUTOMAKE_OPTIONS = foreign

CXXFLAGS = -Wall -Wshadow -Werror -Wcast-qual

bin_PROGRAMS = ipmonit
ipmonit_LDADD = -lpopt
ipmonit_SOURCES = \
  common/SystemUtils.cpp \
  common/StreamLogger.cpp \
  common/SystemLogger.cpp \
  src/NetLinkSocket.cpp \
  src/NetLinkListenClient.cpp \
  src/SocketInterface.cpp \
  binary/ipmonit.cpp
