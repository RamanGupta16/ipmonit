#include "src/NetLinkSocket.hpp"
#include "src/NetLinkListenClient.hpp"

#include <sys/socket.h>
#include <linux/netlink.h> // NETLINK_ROUTE
#include <linux/rtnetlink.h> // RTNLGRP_IPV4_IFADDR

#include <iostream>

using namespace ipmonit;
using namespace std;

int main(int argc, char* argv[])
{
  // Create rtnetlink socket
  auto_ptr<NetLinkSocket> netlink_socket(new NetLinkSocket(NETLINK_ROUTE));
  if(netlink_socket.get() == 0)
  {
    cerr << "Cannot allocate memory for NETLINK socket." << endl;
    return -1;
  }

  // Bind socket to group
  if(netlink_socket->Bind(RTNLGRP_IPV4_IFADDR) < 0)
  {
    cerr << "Cannot bind NETLINK socket." << endl;
    return -1;
  }

  // Create NETLINK client listen for the subscribed group of notification.
  // This client now assumes socket responsibility
  NetLinkListenClient netlink_client(netlink_socket);

  //while(netlink_client.Listen());
  bool status = true;
  while(1)
  {
    status = netlink_client.Listen();
  }
}
