/*
* @Author: simon
* @Date:   2017-08-11 10:29:43
* @Last Modified by:   simon
* @Last Modified time: 2017-09-26 13:11:13
*/
# include <string.h>
# include "Exec.h"


Exec::Exec ()
{

  rfd = -1;
  wfd = -1;
  child = -1;
  argv = NULL;
  args = -1;
}

Exec::Exec (string binary, string path)
{
  pipe (readpipe);		// readpipe[1] <- write end for child0
  pipe (writepipe);		// writepipe[0] <- read end for child


  args = 5;
  argv = (char **) malloc (args * sizeof (char *));

  argv[0] = (char *) binary.c_str ();
  argv[1] = (char *) calloc (sizeof (int), sizeof (char));
  argv[2] = (char *) calloc (sizeof (int), sizeof (char));
  argv[3] = (char *) path.c_str ();
  argv[4] = NULL;

  sprintf (argv[1], "%d", readpipe[1]);
  sprintf (argv[2], "%d", writepipe[0]);

  rfd = readpipe[0];
  wfd = writepipe[1];


  bin = binary;
}

Exec::~Exec ()
{



  string buffer = "\nEOF\n";

  send (buffer);

  if (argv != NULL)
    {
      for (int i = 1; i < args - 2; ++i)
	{
	  if (argv[i] == NULL)
	    continue;
	  free (argv[i]);
	}
      free (argv);
    }

  waitpid (-1, NULL, 0);
  close (readpipe[0]);
  close (readpipe[1]);
  close (writepipe[0]);
  close (writepipe[1]);

}

void
Exec::send (string buffer)
{
  int nbytes = 0;

  nbytes = buffer.length ();

  if (write (wfd, buffer.c_str (), nbytes) < 0)
    {
      cerr << "Parent: short write " << endl;
      perror ("WRITE:");
    }
}

string
Exec::get (string buffer)
{
 
  char readbuffer[1024];

  send (buffer);
  read (rfd, readbuffer, sizeof (readbuffer) - 1);


  if (strstr (readbuffer, "\nEOF\n"))
    return "";

  return readbuffer;
}

void
Exec::start ()
{

  if (bin.empty ())
    return;
  if (argv == NULL)
    return;

  child = fork ();
  if (child == 0)		// child process
    {
      execv (bin.c_str (), argv);
      cerr <<"Wrong path to vparser.py: " << bin << endl;
      perror ("perror: Exec");
    }
}
