# ipmonit
IP Address Monitor on Linux systems.

It uses NETLINK sockets to listen for Kernel notifications when IP Interface (IP Address) are added or deleted. This project is implemented using C++.

Build Commands
===============
./configure && make 

Install (as root)
==================
make install

Compiler
=========
1. Build and tested with gcc version 4.4.7

Testing
========
1. CentOS6
