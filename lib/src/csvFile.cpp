/*
* @Author: simon
* @Date:   2017-09-11 16:37:23
* @Last Modified by:   simon
* @Last Modified time: 2017-09-11 16:38:55
*/


#include "Filehandler.h"


csvFile::csvFile ()
{

}

csvFile::csvFile (string path)
{
  this->path = path;
}

csvFile::~csvFile ()
{

}


/***

	returns the string from position row and column

***/

string csvFile::get (int row, int col)
{
  move_to (row, col);
  if (pos < 0)
    return "";
  string
    tmp;


  read.open (this->path.c_str ());
  read.seekg (pos - 1);
  char
    c;

  while (read.get (c))
    {
      if (c == '\t')
	break;
      tmp.append (1, c);
    }

  read.close ();
  return tmp;
}

/***

	sets the delimiter to del

***/

void
csvFile::setdelimiter (char del)
{
  delimiter = del;
}


/***

	move the filepointer of read to the position given by row col

***/

void
csvFile::move_to (int row, int col)
{
  char c;

  int r_ = 0;

  int c_ = 0;

  read.open (this->path.c_str ());

  while (read.get (c))
    {
      if (r_ == row)
	goto row_found;
      if (c == '\n')
	r_++;
    }

  pos = -2;

  read.close ();
  return;

row_found:

  while (read.get (c))
    {
      if (col == c_)
	goto col_found;
      if (c == '\n')
	break;
      if (c == '\t')
	c_++;
    }

  pos = -3;

  read.close ();
  return;

col_found:
  pos = read.tellg ();

  read.close ();
}

/***

	adds a new row column in case row and col are not defined

***/

void
csvFile::add (int row, int col)
{
  int cctr = 0;

  int rctr = 0;

  char c;

  read.open (path.c_str ());
  write.open ("tmp.csv", std::ofstream::trunc);

  while (read.get (c))
    {
      write << c;
      if (rctr + 1 >= row)
	goto row_found;
      if (c == '\n')
	rctr++;
    }

  while (rctr + 1 < row)
    {
      rctr++;
      write << endl;
    }

row_found:
  cout << "row found " << rctr << endl;

  while (read.get (c))
    {
      if (cctr + 1 == col)
	goto col_found;
      if (c == '\n')
	break;
      if (c == delimiter)
	cctr++;
      write << c;
    }

  while (cctr + 1 < col)
    {
      cctr++;
      write << delimiter;
    }

col_found:

  while (read.get (c))
    write << c;

  read.close ();
  write.close ();

  read.open ("tmp.csv");
  write.open (path.c_str (), std::ofstream::trunc);

  while (read.get (c))
    write << c;

  read.close ();
  write.close ();
}


/***

	changes the string given by position row/col in file read by str

***/

void
csvFile::set (int row, int col, string str)
{
  move_to (row, col);
  if (pos < 0)
    {
      add (row, col);
      move_to (row, col);
      if (pos < 0)
	{
	  cout << "RETURN!!!!" << endl;
	  return;
	}
    }

  char c;

  read.open (path.c_str ());
  write.open ("tmp.csv", std::ofstream::trunc);

  cout << "POS : " << pos << endl;
  while (read.get (c))
    {
      write << c;
      if (read.tellg () == pos - 1)
	break;
    }

  write << (str);
  cout << ">> " << str << endl;
  while (read.get (c))
    {
      if (c == '\n' || c == '\t')
	{
	  write << c;
	  break;
	}
    }

  while (read.get (c))
    write.put (c);

  read.close ();
  write.close ();

  read.open ("tmp.csv");
  write.open (path.c_str (), std::ofstream::trunc);

  while (read.get (c))
    write << c;

  read.close ();
  write.close ();

  return;
}

/***

	inserts the string given by position row/col in file read by str.
	shifts the following strings by one position.

***/

void
csvFile::insert (int row, int col, string str)
{
  move_to (row, col);

  if (pos < 0)
    {

    }
}
