#include "NetLinkListenClient.hpp"
#include "common/SystemLogger.hpp"

#include <linux/rtnetlink.h>
#include <netinet/in.h> // INET_ADDRSTRLEN INET6_ADDRSTRLEN
#include <arpa/inet.h> // inet_ntop
#include <errno.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <sstream>

using namespace ipmonit;
using namespace std;

namespace
{
  string familyIndexToName(uint8_t index)
  {
    switch(index)
    {
      case AF_INET:
        return "AF_INET";
      case AF_INET6:
        return "AF_INET6";
      case AF_UNIX:
        return "AF_UNIX";
      case AF_BRIDGE:
        return "AF_BRIDGE";
      case AF_UNSPEC:
        return "AF_UNSPEC";
      default:
        return "Unknown";
    }
  }
}

NetLinkListenClient::
#if __cplusplus >= 201103L
    NetLinkListenClient(std::unique_ptr<NetLinkSocket> socket)
  : mpNetLinkSocket(std::move(socket))
#else
    NetLinkListenClient(std::auto_ptr<NetLinkSocket> socket)
  : mpNetLinkSocket(socket)
#endif
{
  if(mpNetLinkSocket.get() == 0)
  {
    SystemLogger::LogError() << "No NetLink socket found";
    // TODO: Throw exception
  }
}

bool
NetLinkListenClient::
Listen()
{
  if(mpNetLinkSocket.get() == 0)
  {
    // TODO: Throw exception
    return false;
  }

  // Recv NETLINK message from kernel
  static uint8_t recv_buffer[8192] = {0}; // Big buffer to avoid MSG_TRUNC
  struct iovec iov = {recv_buffer, sizeof(recv_buffer)};

  struct sockaddr_nl peer; // Kernel
  memset(&peer, 0, sizeof(peer));
  peer.nl_family = AF_NETLINK;

  struct msghdr msg = {&peer, sizeof(peer), &iov, 1, NULL, 0, 0};
  struct nlmsghdr *nh = 0;
  struct ifaddrmsg* if_addr = 0;

  string address;
  string label;

  unsigned int len = recvmsg(mpNetLinkSocket->GetFd(), &msg, 0);
  if(len < 0)
  {
    if(errno == EINTR || errno == EAGAIN)
    {
      return true;
    }
    else
    {
      SystemLogger::LogError()
        << "recvmsg() failure for NETLINK socket:"
        << strerror(errno);
      mpNetLinkSocket->Close();
      return false;
    }
  }
  else if (len == 0)
  {
    SystemLogger::LogError() << "peer indicates EoF (closure) for NETLINK socket";
    mpNetLinkSocket->Close();
    return false;
  }

  for(nh = (struct nlmsghdr*) recv_buffer; NLMSG_OK(nh, len); nh = NLMSG_NEXT(nh, len))
  {
    if(nh->nlmsg_type == NLMSG_DONE)
    {
      break;
    }

    if(nh->nlmsg_type == NLMSG_ERROR)
    {
      SystemLogger::LogError()
        << "peer indicates EoF (closure) for NETLINK socket"
        << strerror(errno);
      break;
    }

    if(nh->nlmsg_type == RTM_NEWLINK)
    {
      //struct ifinfomsg* ifi = (struct ifinfomsg*)NLMSG_DATA(nh);
    }

    if(nh->nlmsg_type == RTM_DELLINK)
    {
      //struct ifinfomsg* ifi = (struct ifinfomsg*)NLMSG_DATA(nh);
    }

    if(nh->nlmsg_type == RTM_NEWADDR)
    {
      if_addr = (struct ifaddrmsg*)NLMSG_DATA(nh);
      handleAddress(if_addr, nh, &address, &label);
      HandleIpAddressAdded(address, label);
    }

    if(nh->nlmsg_type == RTM_DELADDR)
    {
      if_addr = (struct ifaddrmsg*)NLMSG_DATA(nh);
      handleAddress(if_addr, nh, &address, &label);
      HandleIpAddressDeleted(address, label);
    }
  }

  return true;
}

void
NetLinkListenClient::
HandleIpAddressAdded(const string& address, const string& label)
{
  SystemLogger::LogInfo() << "Added Address " << address << " Interface " << label;
}

void
NetLinkListenClient::
HandleIpAddressDeleted(const string& address, const string& label)
{
  SystemLogger::LogInfo() << "Deleted Address " << address << " Interface " << label;
}

string
NetLinkListenClient::
handleAddress(const struct ifaddrmsg* ifAddr, const struct nlmsghdr* nh,
              string* pOutAddress, string* pOutLabel)
{
  if(!ifAddr || !nh)
  {
    SystemLogger::LogDebug("Error: missing data");
    return "";
  }

  stringstream s;
  s << "Family:" << familyIndexToName(ifAddr->ifa_family);
  //s << " LinkIndex:" << ifAddr->ifa_index;

  // Convert from ifaddrmsg to rtattr
  struct rtattr* rta = IFA_RTA(ifAddr);

  // Iterate over rtattr
  int len = nh->nlmsg_len - NLMSG_LENGTH(sizeof(*ifAddr));
  while(RTA_OK(rta, len))
  {
    if(rta->rta_type <= IFA_MAX)
    {
      switch(rta->rta_type)
      {
        case IFA_UNSPEC:
        {
          break;
        }
        case IFA_ADDRESS:
        {
          break;
        }
        case IFA_LOCAL:
        {
          int rta_length = RTA_PAYLOAD(rta);
          if(rta_length == 4 && ifAddr->ifa_family == AF_INET)
          {
            struct in_addr* ipv4_addr = static_cast<struct in_addr*>(RTA_DATA(rta));
            char ipv4_address[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &ipv4_addr->s_addr, ipv4_address, sizeof(ipv4_address));
            s << " IPv4 Address:" << ipv4_address;
            *pOutAddress = ipv4_address;
          }
          else if(rta_length == 16 && ifAddr->ifa_family == AF_INET6)
          {
            struct in6_addr* ipv6_addr = static_cast<struct in6_addr*>(RTA_DATA(rta));
            char ipv6_address[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, &ipv6_addr->s6_addr, ipv6_address, sizeof(ipv6_address));
            s << " IPv6 Address:" << ipv6_address;
            *pOutAddress = ipv6_address;
          }
          break;
        }
        case IFA_LABEL:
        {
          const char* label = static_cast<const char*>(RTA_DATA(rta));
          s << " Label:" << label;
          *pOutLabel = label;
          break;
        }
        case IFA_BROADCAST:
        {
          struct in_addr* ipv4_addr = static_cast<struct in_addr*>(RTA_DATA(rta));
          char ipv4_address[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &ipv4_addr->s_addr, ipv4_address, sizeof(ipv4_address));
          s << " Broadcast Address:" << ipv4_address;
          break;
        }
        case IFA_ANYCAST:
        {
          break;
        }
        case IFA_CACHEINFO:
        {
          break;
        }
        case IFA_MULTICAST:
        {
          break;
        }
        default:
        {
          break;
        }
      }
    }
    else
    {
      SystemLogger::LogError() << "Unknown RTA TYPE:" << rta->rta_type;
    }
    rta = RTA_NEXT(rta, len);
  }

  return s.str();
}

