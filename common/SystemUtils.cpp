#include "SystemUtils.hpp"
#include "SystemLogger.hpp"

#include <unistd.h>
#include <errno.h>
#include <stdio.h> // fflush
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open
#include <linux/limits.h> // PATH_MAX
#include <string.h> // strerror
#include <stdlib.h> // exit

#include <iostream>
#include <sstream>

using namespace ipmonit;
using namespace std;

#define PID_FILE "/var/run/ipmonit.pid"

string
SystemUtils::
GetExePath()
{
  char result[ PATH_MAX ] = {0};
  ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
  return std::string( result, (count > 0) ? count : 0 );
}

void
SystemUtils::
Daemonify(const string& serverDir)
{
  // Create child process
  pid_t status = fork();
  if(status < 0)
  {
    cerr << "Error on fork: " << strerror(errno) << endl;
    exit(-1);
  }
  else if(status > 0)
  {
    exit(0); // Stop the parent process
  }

  /* Child Process */

  // Set unmask
  umask(022);

  // Set new Session
  if(setsid() < 0)
  {
    cerr << "Error on setsid(): " << strerror(errno) << endl;
    exit(-1);
  }

  // Change the current working directory to (default) root " / "
  string server_dir("/");
  if(!serverDir.empty())
  {
    server_dir = serverDir;
  }

  // Change the current working directory
  if(chdir(server_dir.c_str()) < 0)
  {
    cerr << "Error in changing dir to: " << server_dir
         << " while daemonizing. Failure: " << strerror(errno)
         << endl;
    exit(-1);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}

int
SystemUtils::
LockPidFile()
{
  int fd = open(PID_FILE, O_CREAT | O_RDWR, 0640);
  if(fd < 0)
  {
    SystemLogger::LogError("Cannot open " PID_FILE " file");
    exit(-1);
  }

  if(lockf(fd, F_TLOCK, 0) < 0)
  {
    SystemLogger::LogError("Possibly another instance of ipmonit already running. Exiting...");
    close(fd);
    exit(-1);
  }

  stringstream s;
  s << getpid();
  if(write(fd, s.str().c_str(), s.str().size()) < 0)
  {
    SystemLogger::LogError("Cannot write PID to " PID_FILE " Exiting...");
    close(fd);
    exit(-1);
  }

  return fd;
}

void
SystemUtils::
CloseLockFile(int fd)
{
  if(fd < 0)
  {
    return;
  }

  lseek(fd, 0, SEEK_SET);

  char buff[32] = {0};
  if(read(fd, buff, 32) < 0)
  {
    SystemLogger::LogError("Cannot read from " PID_FILE);
    return;
  }

  // Match PID with the value in lock file
  if(getpid() == atoi(buff))
  {
    SystemLogger::LogInfo("Deleting lock file " PID_FILE);
    close(fd);

    // Delete locked file
    if(unlink(PID_FILE) < 0)
    {
      SystemLogger::LogError("Deleting lock file " PID_FILE " failed!");
    }
  }
  else
  {
    close(fd);
  }
}
