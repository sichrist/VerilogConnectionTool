/*
* @Author: simon
* @Date:   2017-08-11 16:03:41
* @Last Modified by:   simon
* @Last Modified time: 2017-08-11 16:05:32
*/
/*
* @Author: simon
* @Date:   2017-08-11 14:36:29
* @Last Modified by:   simon
* @Last Modified time: 2017-08-11 16:03:41
*/

#include "thread.h"


using namespace std;

Thread::~Thread ()
{

}

Thread::Thread (void *arg)
{
  this->arg = arg;
  _pthread = NULL;
  mainloop = true;
}

Thread::Thread (void (*func) (void *))
{
  _pthread = func;
  arg = NULL;
  mainloop = true;
}

Thread::Thread (void (*func) (void *), void *arg)
{
  this->arg = arg;
  _pthread = func;
  mainloop = true;
}

Thread::Thread ()
{
  _pthread = NULL;
  mainloop = true;
}

void
Thread::start (void *arg)
{
  if (_pthread == NULL)
    {
      cout << " Pthread: _pthread = NULL" << endl;
      return;
    }
  pthread_create (&tid, NULL, (void *(*)(void *)) _pthread, arg);
}

void
Thread::start (void (*func) (void *))
{
  if (func == NULL)
    {
      cout << "func = NULL" << endl;
      return;
    }

  pthread_create (&tid, NULL, (void *(*)(void *)) func, arg);
}

void
Thread::start (void (*func) (void *), void *arg)
{
  if (func == NULL)
    {
      cout << "func = NULL" << endl;
      return;
    }
  pthread_create (&tid, NULL, (void *(*)(void *)) func, arg);
}

void
Thread::start ()
{
  if (_pthread == NULL)
    {
      cout << "func = NULL" << endl;
      return;
    }

  pthread_create (&tid, NULL, (void *(*)(void *)) _pthread, arg);
}

void
Thread::join ()
{
  pthread_join (tid, NULL);
}

void
Thread::stop ()
{
  mainloop = false;
}

int
Thread::detach ()
{
  return pthread_detach (tid);
}
