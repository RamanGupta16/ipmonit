#include "src/NetLinkSocket.hpp"
#include "src/NetLinkListenClient.hpp"
#include "common/SystemLogger.hpp"
#include "common/SystemUtils.hpp"

#include <sys/socket.h>
#include <linux/netlink.h> // NETLINK_ROUTE
#include <linux/rtnetlink.h> // RTNLGRP_IPV4_IFADDR
#include <popt.h>
#include <stdlib.h> // exit
#include <unistd.h> // getpid()

#include <iostream>
#include <sstream>

using namespace ipmonit;
using namespace std;

int main(int argc, const char* argv[])
{
  int run_as_daemon = 0; // Set to 1 if --daemon option provided
  struct poptOption cmd_line_options[] =
  {
    {"daemon", 'b', POPT_ARG_NONE, &run_as_daemon, 0,
     "Run as daemon with root login and single instance. Using syslog for logging into file /var/log/messages", 0},
    POPT_AUTOHELP
    POPT_TABLEEND
  };

  poptContext optcon = poptGetContext(0, argc, argv, cmd_line_options, 0);
  int rc = poptGetNextOpt(optcon);
  if(rc < -1)
  {
    cerr << "Error with option: "
         << poptBadOption(optcon, POPT_BADOPTION_NOALIAS) << ": "
         << poptStrerror(rc) << endl;
    exit(1);
  }

  int lockfile_fd = -1;
  if(run_as_daemon)
  {
    const string server_dir("/tmp");
    SystemUtils::Daemonify(server_dir);
    SystemLogger::SetLogType(SYSLOG);

    // Single instance daemon
    lockfile_fd = SystemUtils::LockPidFile();
  }
  else
  {
    SystemLogger::SetLogType(TERMINAL);
  }

  // Create rtnetlink socket
#if __cplusplus >= 201402L
  unique_ptr<NetLinkSocket> netlink_socket = make_unique<NetLinkSocket>(NETLINK_ROUTE);
#elif __cplusplus >= 201103L
  unique_ptr<NetLinkSocket> netlink_socket{new NetLinkSocket(NETLINK_ROUTE)};
#else
  auto_ptr<NetLinkSocket> netlink_socket(new NetLinkSocket(NETLINK_ROUTE));
#endif

  if(netlink_socket.get() == 0)
  {
    SystemLogger::LogError() << "Cannot allocate memory for NETLINK socket.";
    return -1;
  }

  // Bind socket to group
  if(netlink_socket->Bind(RTNLGRP_IPV4_IFADDR) < 0)
  {
    SystemLogger::LogError() << "Cannot bind NETLINK socket.";
    return -1;
  }

  // Create NETLINK client listen for the subscribed group of notification.
  // This client now assumes socket responsibility
#if __cplusplus >= 201103L
  NetLinkListenClient netlink_client(std::move(netlink_socket));
#else
  NetLinkListenClient netlink_client(netlink_socket);
#endif

  SystemLogger::LogInfo() << "Started ipmonit[PID:" << getpid() << "]...";

  while(netlink_client.Listen());

  // Close the Lock file
  SystemUtils::CloseLockFile(lockfile_fd);

  return 0;
}
