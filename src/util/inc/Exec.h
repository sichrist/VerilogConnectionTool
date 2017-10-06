/*
* @Author: simon
* @Date:   2017-08-11 16:05:19
* @Last Modified by:   simon
* @Last Modified time: 2017-08-24 08:58:13
*/

#ifndef EXEC_H
#define EXEC_H

# include <unistd.h>
# include <iostream>
# include <string>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>		// Waitpid

using namespace std;

class Exec
{

private:
  int readpipe[2];
  int writepipe[2];
  int rfd;
  int wfd;
  pid_t child;
  char **argv;
  int args;

  string bin;

public:
    Exec (string binary, string path);
    Exec ();
   ~Exec ();
  void start ();
  void setregex (string regex);
  void startThread ();
  void send (string buffer);
  string get (string buffer);

};


# endif
