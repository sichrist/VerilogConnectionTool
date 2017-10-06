/*
* @Author: simon
* @Date:   2017-09-13 13:17:32
* @Last Modified by:   simon
* @Last Modified time: 2017-09-25 14:02:16
*/

# include "Path.h"


/***

  returns true if a directory exists

***/

inline bool
exists_dir (const std::string & name)
{
  DIR *dir = opendir (name.c_str ());
  if (dir)
    {
      /* Directory exists. */
      closedir (dir);
      return true;
    }
  return false;
}

/***

  returns true if a file exists

***/

inline bool
exists_file (const std::string & name)
{
  if (FILE * file = fopen (name.c_str (), "r"))
    {
      fclose (file);
      return true;
    }
  else
    {
      return false;
    }
}

Path::Path( string path, char delim ) : Cutter( path, delim )
{
  set( path, delim );
}

Path::Path( string path ) : Cutter( path, '/' )
{
  set( path, '/' );
}

Path::~Path()
{
}

void Path::set( string path, char delim )
{
  data      = path;
  delimiter = delim;
  end();
  filename  = next();
  end();
  nextstep();
  folderpath= nextstep() + "/";
}

string Path::getfilename()
{
  return filename;
}

string Path::getfolder()
{
  return folderpath;
}

string Path::getfullpath()
{
  return data;
}

bool Path::fileExist()
{
  return exists_file(data);
}

bool Path::dirExist()
{
  return exists_dir(folderpath);
}

bool Path::createFile()
{
  return false;
}

bool Path::createFolder()
{
  return false;
}
