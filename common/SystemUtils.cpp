#include "SystemUtils.hpp"
#include <unistd.h>
#include <errno.h>
#include <stdio.h> // fflush
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/limits.h> // PATH_MAX
#include <string.h> // strerror
#include <stdlib.h> // exit

#include <iostream>

using namespace ipmonit;
using namespace std;

string
SystemUtils::
GetExepath()
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
