/*
* @Author: simon
* @Date:   2017-08-10 10:58:52
* @Last Modified by:   simon
* @Last Modified time: 2017-09-20 11:08:03
*/
# include "Filehandler.h"


Filehandler::Filehandler ()
{
  start = 0;
  end = 0;
  path = "";
}

Filehandler::Filehandler (string path)
{
  start = 0;
  end = 0;
  this->path = path;
}

Filehandler::~Filehandler ()
{

}

void
Filehandler::setpath (string path)
{
  this->path = path;
}

string Filehandler::getpath ()
{
  return this->path;
}

long
Filehandler::getstart ()
{
  return this->start;
}

long
Filehandler::getend ()
{
  return this->end;
}

/***

	

***/

void
Filehandler::ffwd (string end)
{
  char c;

  int ctr = 0;

  int len = end.length ();

  while (read.get (c))
    {
      if (c == end[ctr])
	ctr++;
      else if (c == end[0])
	ctr = 1;
      else
	ctr = 0;
      if (ctr == len)
	break;
    }
}

void
Filehandler::fullcpy (string path)
{
  start = 0;
  end = -1;
  write.open (path.c_str (), fstream::trunc);
  write.close ();
  cpy (path);

}

/***

	cpy will copy the full File read to the File path

***/

void
Filehandler::cpy (string path)
{
  if (this->path.empty ())
    return;

  if (start < 0 || end == 0)
    return;
  //cerr <<"--" <<path<< " " <<this->path <<endl;

  read.open ((this->path).c_str ());
  write.open (path.c_str (), fstream::app);
  char c = 0;

  read.seekg (start);

  if (end != -1)
    {
      while (read.tellg () < end)
	{
	  read.get (c);
	  write.put (c);
	}
    }

  else
    {
      while (read.tellg () != EOF)
	{
	  read.get (c);
	  write.put (c);
	}
    }

  read.close ();
  write.close ();
}

/***

	seek is looking for the string str in the file read and stops,
	when end is read. If end is NUll, seek will search the whole file.

***/

bool Filehandler::seek (string str, string end)
{
  if (this->path.empty ())
    return false;

  char
    c;

  int
    strctr = 0, endctr = 0;

  long
    strlen,
    endlen;

  strlen = str.length ();
  endlen = end.length ();
  this->start = -1;
  this->end = -1;
  read.open (path.c_str ());

  while (read.get (c))
    {

      if (strlen == strctr)
	{
	  this->end = read.tellg ();
	  this->end--;
	  this->start = this->end - strlen;
	  read.close ();
	  return true;
	}

      if (c == '/')
	while (c != '\n')
	  read.get (c);
      if (str[strctr] == c)
	{
	  strctr++;
	}
      else if (str[0] == 'c')
	strctr = 1;
      else
	{
	  strctr = 0;
	}
      if (!end.empty ())
	{
	  if (end[endctr] == c)
	    endctr++;
	  else if (end[0] == c)
	    endctr = 1;
	  else
	    {
	      endctr = 0;
	    }

	  if (endlen == endctr)
	    {
	      break;
	    }
	}
    }

  read.close ();
  return false;
}
