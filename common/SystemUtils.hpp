#ifndef __ipmonit_common_SystemUtils_hpp
#define __ipmonit_common_SystemUtils_hpp

#include <string>
#include <fstream>  // ofstream

namespace ipmonit
{
  class SystemUtils
  {
  public:
    static std::string GetExePath();
    static void Daemonify(const std::string& serverDir);
    static int LockPidFile();
    static void CloseLockFile(int fd);
  };
}

#endif // __ipmonit_common_SystemUtils_hpp
