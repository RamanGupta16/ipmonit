#include "StreamLogger.hpp"
#include "SystemLogger.hpp"

using namespace ipmonit;
using namespace std;

StreamLogger::
StreamLogger(int priority, const string& append)
  : mPriority(priority),
    mStream(append)
{
}

StreamLogger::
StreamLogger(const StreamLogger& rhs)
  : mPriority(rhs.mPriority)
{
  mStream << rhs.mStream.str();
}

StreamLogger&
StreamLogger::
operator = (const StreamLogger& rhs)
{
  if(this != &rhs)
  {
    mPriority = rhs.mPriority;
    mStream.str().clear();
    mStream << rhs.mStream.str();
  }
  return *this;
}

StreamLogger::
~StreamLogger()
{
  SystemLogger::Log(mPriority, mStream.str());
}
