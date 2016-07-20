#ifndef __ipmonit_src_NetLinkListenClient_hpp
#define __ipmonit_src_NetLinkListenClient_hpp

#include "NetLinkSocket.hpp"

#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/if_addr.h>

#include <string>
#include <memory>

namespace ipmonit
{
	class NetLinkListenClient
	{
	public:
    // @param socket: Own the responsibility of provided socket
		NetLinkListenClient(std::auto_ptr<NetLinkSocket> socket);

    // Listen the subscribed group for kernel notification
		bool Listen();

    // User shud override this function to listen for IP Address added
    // @parameter address: IP Address added in dotted decimal notation
    // @parameter label: Name of interface added
		virtual void HandleIpAddressAdded(const std::string& address, const std::string& label);

    // User shud override this function to listen for IP Address deleted
    // @parameter address: IP Address deleted in dotted decimal notation
    // @parameter label: Name of interface deleted
		virtual void HandleIpAddressDeleted(const std::string& address, const std::string& label);

  private:
    std::string handleAddress(const struct ifaddrmsg* ifAddr, const struct nlmsghdr* nh,
                              std::string* pOutAddress, std::string* pOutLabel);

	private:
		std::auto_ptr<NetLinkSocket> mpNetLinkSocket;
	};
}

#endif // __ipmonit_src_NetLinkListenClient_hpp
