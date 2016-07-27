#ifndef __ipmonit_common_SystemLogger_hpp
#define __ipmonit_common_SystemLogger_hpp

#include <string>

namespace ipmonit
{
  enum LogStrategy
  {
    TERMINAL, // log to terminal
    SYSLOG    // log to syslog
  };

  class SystemLogger
  {
  public:
    static void SetLogType(LogStrategy strategy);

  public:
    static void LogInfo(const std::string& msg);
    static void LogDebug(const std::string& msg);
    static void LogError(const std::string& msg);

  private:
    static LogStrategy mLoggerType;
  };
}

#endif // __ipmonit_common_SystemLogger_hpp
