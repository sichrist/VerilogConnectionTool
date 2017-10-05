/*
* @Author: simon
* @Date:   2017-09-13 13:17:32
* @Last Modified by:   simon
* @Last Modified time: 2017-09-25 14:02:16
*/

# include "Controller.h"

/// Splits a path in folder and filename

Path::Path ()
{
  path = "";
  pathfolder = "";
  filename = "";
  delim = '/';
}

Path::Path (string path_pa)
{
  setpath (path_pa);
}

Path::Path (string path_pa, char delim_pa)
{
  setpath (path_pa, delim_pa);
}

Path::~Path ()
{

}

void
Path::process ()
{
  Parser parser;
  parser.setstring (path);
  parser.setdelimiter (delim);
  parser.parse ();
  parser.getnext ();
  pathfolder = parser.getnext ();

  parser.singleparse ();
  filename = parser.getnextbw ();
}

void
Path::setpath (string path_pa)
{
  path = path_pa;
  process ();

}

void
Path::setpath (string path_pa, char delim_pa)
{
  this->path = path_pa;
  this->delim = delim_pa;
  process ();
}

string
Path::getpath ()
{
  return path;
}

string
Path::getpathfolder ()
{
  return pathfolder;
}

string
Path::getfilename ()
{
  return filename;
}

bool
Path::exists ()
{
  return exists_file (path);
}

bool
Path::exists (string path)
{
  return exists_file (path + filename);
}
