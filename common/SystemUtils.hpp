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
    static void Daemonify(const std::string& fileName,
                          const std::string& dirName,
                          std::ofstream* fout);
  };
}

#endif // __ipmonit_common_SystemUtils_hpp
