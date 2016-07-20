#ifndef __ipmonit_src_SocketInterface_hpp
#define __ipmonit_src_SocketInterface_hpp

namespace ipmonit
{
  class SocketInterface
  {
  public:
    ~SocketInterface();
	  virtual int GetFd() const = 0;
  	virtual int Bind(int group) = 0;
	  virtual void Close() = 0;
  };
}

#endif // __ipmonit_src_SocketInterface_hpp
