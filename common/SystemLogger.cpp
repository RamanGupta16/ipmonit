#include "SystemLogger.hpp"

#include <string.h>
#include <syslog.h>

#include <iostream>

using namespace ipmonit;

// remove trailing '\n' char
#define LOG_ON_TERMINAL(msg) \
  time_t now = time(0); \
  char* foo = ctime(&now); \
  foo[strlen(foo) -1] = 0; \
  std::cout << foo << " ipmonit [" << getpid() << "]: " << msg << std::endl;

LogStrategy SystemLogger::mLoggerType = TERMINAL;

void
SystemLogger::
SetLogType(LogStrategy strategy)
{
  mLoggerType = strategy;
}

void
SystemLogger::
LogInfo(const std::string& msg)
{
  if(mLoggerType == TERMINAL)
  {
    LOG_ON_TERMINAL(msg);
  }
  else
  {
    openlog(0, LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, msg.c_str());
    closelog();
  }
}

void
SystemLogger::
LogDebug(const std::string& msg)
{
  if(mLoggerType == TERMINAL)
  {
    LOG_ON_TERMINAL(msg);
  }
  else
  {
    openlog(0, LOG_PID, LOG_DAEMON);
    syslog(LOG_DEBUG, msg.c_str());
    closelog();
  }
}

void
SystemLogger::
LogError(const std::string& msg)
{
  if(mLoggerType == TERMINAL)
  {
    LOG_ON_TERMINAL(msg);
  }
  else
  {
    openlog(0, LOG_PID, LOG_DAEMON);
    syslog(LOG_ERR, msg.c_str());
    closelog();
  }
}
