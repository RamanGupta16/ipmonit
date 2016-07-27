#ifndef __ipmonit_common_SystemUtils_hpp
#define __ipmonit_common_SystemUtils_hpp

#include <string>
#include <fstream>  // ofstream

namespace ipmonit
{
  class SystemUtils
  {
  public:
    static std::string GetExepath();
    static void Daemonify(const std::string& serverDir);
  };
}

#endif // __ipmonit_common_SystemUtils_hpp
