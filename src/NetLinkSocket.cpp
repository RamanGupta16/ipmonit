#include "NetLinkSocket.hpp"
#include "common/SystemLogger.hpp"

#include <sys/socket.h>
#include <linux/netlink.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <sstream>

using namespace ipmonit;
using namespace std;

NetLinkSocket::
NetLinkSocket(int netlinkFamily)
  : mSocketFd(socket(AF_NETLINK, SOCK_RAW, netlinkFamily))
{
  if(mSocketFd < 0)
  {
    stringstream s;
    s << "Cannot open NETLINK socket for family "
      << netlinkFamily << " Error:" << strerror(errno);
    SystemLogger::LogError(s.str());
  }
}

NetLinkSocket::
~NetLinkSocket()
{
  Close();
}

int
NetLinkSocket::
Bind(int group)
{
  int status = -1;

  // Get Multicast group bitset
  if(group > 31 || group < 0)
  {
    stringstream s;
    s << "Mcast group " << group
      << " is out of range. Use setsockopt for this mcast group ";
    SystemLogger::LogError(s.str());
    return status;
  }

  int mcast_group = group ? (1 << (group -1)) : 0;

  /* TODO: if already bound then unbound, close socket and rebind */

  // Create local netlink socket address
  struct sockaddr_nl local;
  memset(&local, 0, sizeof(local));
  local.nl_family = AF_NETLINK;
  local.nl_pid = getpid();
  local.nl_groups = mcast_group /*RTNLGRP_IPV4_IFADDR*/;

  // Bind the socket
  status = bind(mSocketFd, (struct sockaddr*)&local, sizeof(local));
  if(status < 0)
  {
    stringstream s;
    s << "Cannot bind NETLINK socket. Error:" << strerror(errno);
    SystemLogger::LogError(s.str());
    Close();
    return status;
  }

  // Verify socket bounded correctly
  socklen_t addr_len = sizeof(local);
  status = getsockname(mSocketFd, (struct sockaddr*)&local, &addr_len);
  if(status < 0)
  {
    stringstream s;
    s << "Cannot getsockname for bound NETLINK socket. Error:" << strerror(errno);
    SystemLogger::LogError(s.str());
    Close();
    return status;
  }
  if(addr_len != sizeof(local))
  {
    stringstream s;
    s << "Wrong address length: " << addr_len;
    SystemLogger::LogError(s.str());
    Close();
    status = -1;
    return status;
  }
  if(local.nl_family != AF_NETLINK)
  {
    stringstream s;
    s << "Wrong address family: " << local.nl_family;
    SystemLogger::LogError(s.str());
    Close();
    status = -1;
    return status;
  }

  return status;
}

int
NetLinkSocket::
GetFd() const
{
  return mSocketFd;
}

void
NetLinkSocket::
Close()
{
  close(mSocketFd);
  mSocketFd = -1;
}
