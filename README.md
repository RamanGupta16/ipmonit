# ipmonit
IP Address Monitor on Linux systems.

It uses NETLINK sockets to listen for Kernel notifications when IP Interface (IP Address) are added or deleted. This project is implemented using C++.

Build Commands
===============
1) ./configure && make 
2) To build fresh configure script from configure.ac: autoreconf -vfis

Install (as root)
==================
make install

Compiler
=========
1) Build and tested with gcc version 4.4.7

Testing
=========
1) Tested on CentOS6:
   1.1) Start ipmonit from command prompt.
   1.2) Add virtual network interface with some IP Address:  ifconfig eth0:0 10.168.150.78 up
   1.3) The ipmonit will report: "Added Address: 10.168.150.78 Interface: eth0:0"
   1.4) When same interface is deleted then ipmonit reports: "Deleted Address: 10.168.150.78 Interface: eth0:0"


