#include "SystemLogger.hpp"

#include <string.h>
#include <syslog.h>
#include <unistd.h> // getpid()
#include <iostream>

using namespace ipmonit;
using namespace std;

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

StreamLogger
SystemLogger::
LogInfo(const string& append)
{
  return StreamLogger(LOG_INFO, append);
}

StreamLogger
SystemLogger::
LogDebug(const string& append)
{
  return StreamLogger(LOG_DEBUG, append);
}

StreamLogger
SystemLogger::
LogError(const string& append)
{
  return StreamLogger(LOG_ERR, append);
}

void
SystemLogger::
Log(int priority, const string& msg)
{
  if(priority < LOG_EMERG || priority > LOG_DEBUG)
  {
    priority = LOG_INFO;
  }

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
