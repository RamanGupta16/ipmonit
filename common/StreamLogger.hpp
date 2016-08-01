#ifndef __ipmonit_common_StreamLogger_hpp
#define __ipmonit_common_StreamLogger_hpp

#include <sstream>
#include <string>

namespace ipmonit
{
  class StreamLogger
  {
  public:
    StreamLogger(int priority, const std::string& append = std::string());
    StreamLogger(const StreamLogger& rhs);
    StreamLogger& operator = (const StreamLogger& rhs);

    ~StreamLogger();

    template<typename T>
    StreamLogger& operator << (const T& val)
    {
      mStream << val;
      return *this;
    }

  private:
    int mPriority;
    std::stringstream mStream;
  };
}

#endif // __ipmonit_common_StreamLogger_hpp
