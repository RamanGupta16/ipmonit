# ipmonit
IP Address Monitor on Linux systems.

It uses NETLINK sockets to listen for Kernel notifications when IP Interface (IP Address) are added or deleted. This project is implemented using C++.

## Build Commands
* ./configure && make 
* To build fresh configure script from configure.ac: autoreconf -vfis

## Install (as root)
* make install

## Compiler
* Build and tested with gcc version 4.4.7
* Build and tested with gcc version 8.3.1

## Usage
````````````
$ ipmonit --help
Usage: ipmonit [OPTION...]
   -b, --daemon      Run as daemon process using syslog for logging into file /var/log/messages
````````````
## Testing
* Tested on CentOS6 and CentOS8:
   1. Start ipmonit from command prompt.
   2. Add virtual network interface with some IP Address:  `ifconfig eth0:0 10.168.150.78 up` or `ifconfig enp0s3:0 10.168.150.78 up`
   3. The ipmonit will report: "Added Address: 10.168.150.78 Interface: eth0:0" or "Added Address: 10.168.150.78 Interface: enp0s3:0"
   4. Delete the virtual network interface: `ifconfig eth0:0 10.168.150.78 down` or `ifconfig enp0s3:0 10.168.150.78 down`
   5. When same interface is deleted then ipmonit reports: "Deleted Address: 10.168.150.78 Interface: eth0:0" or "Deleted Address: 10.168.150.78 Interface: enp0s3:0"


