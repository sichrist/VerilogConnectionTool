/*
* @Author: simon
* @Date:   2017-08-11 16:05:15
* @Last Modified by:   simon
* @Last Modified time: 2017-08-11 16:16:34
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>

class Thread
{

protected:
  pthread_t tid;
  void *arg;
  void (*_pthread) (void *);
  bool mainloop;
public:
   ~Thread ();
    Thread ();
    Thread (void *arg);
    Thread (void (*func) (void *));
    Thread (void (*func) (void *), void *arg);
  void start (void *arg);
  void start (void (*func) (void *));
  void start (void (*func) (void *), void *arg);
  void start ();
  int detach ();
  void stop ();
  void join ();
};
