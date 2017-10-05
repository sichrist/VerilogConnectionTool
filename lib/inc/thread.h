/*
* @Author: simon
* @Date:   2017-08-11 16:05:18
* @Last Modified by:   simon
* @Last Modified time: 2017-08-14 09:38:38
*/

# ifndef THREAD_H
# define THREAD_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <iostream>


// NOT USED!
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
  void stop ();
  void join ();

  int detach ();

};

# endif
