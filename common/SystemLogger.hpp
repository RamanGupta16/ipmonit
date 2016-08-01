#ifndef __ipmonit_common_SystemLogger_hpp
#define __ipmonit_common_SystemLogger_hpp

#include "StreamLogger.hpp"

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
    // @param append Informational Append string
    static StreamLogger LogInfo(const std::string& append = std::string());
    static StreamLogger LogDebug(const std::string& append = std::string());
    static StreamLogger LogError(const std::string& append = std::string());

    static void Log(int priority, const std::string& msg);

  private:
    static LogStrategy mLoggerType;
  };
}

#endif // __ipmonit_common_SystemLogger_hpp
