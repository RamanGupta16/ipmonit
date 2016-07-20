#ifndef __ipmonit_src_NetLinkSocket_hpp
#define __ipmonit_src_NetLinkSocket_hpp

#include "SocketInterface.hpp"

namespace ipmonit
{
  class NetLinkSocket : public SocketInterface
  {
  public:
	  NetLinkSocket(int netlinkFamily);
	  ~NetLinkSocket(); 

  public: // SocketInterface
	  int GetFd() const;

  	// @param group: Multicast group to bind. Zero means no mcast group
	  // @return     : bind return status or -1 in case of error 
  	int Bind(int group);

    // Close the socket
	  void Close();

  private:
	  int mSocketFd;
  };
}

#endif // __ipmonit_src_NetLinkSocket_hpp
