
/*
* @Author: simon
* @Date:   2017-08-11 16:16:48
* @Last Modified by:   simon
* @Last Modified time: 2017-08-29 16:28:22
*/
# ifndef FILEHANDLER_H
# define FILEHANDLER_H

# include <iostream>
# include <string>
# include <fstream>
# include "Exec.h"
# include "Cutter.h"
# include <sstream>
# include <boost/regex.hpp>
# include <boost/iostreams/device/mapped_file.hpp>
# include "IOcon.h"

using namespace  boost::iostreams;
using namespace  std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class Filehandler
{
protected:
  ifstream    read;
  ofstream    write;
  string    path;
  long    start;
  long    end;

public:
  Filehandler ();
  Filehandler (string path);
  ~Filehandler ();
  void  setpath (string path);
  void  cpy (string path);
  void  fullcpy (string path);
  void  ffwd (string end);

  bool  seek (string str, string end);
  long  getstart ();
  long  getend ();
  string  getpath ();
};

class  cfgFile:  public  Filehandler
{
  map <string,map < int,string > > input;
public:  void
  parse ();
};

class  csvFile:  public  Filehandler
{
private:

  char    delimiter;
  long    pos;

public:

  csvFile ();
  csvFile (string path);
  ~csvFile ();
  string  get (int row, int col);
  void  setdelimiter (char del);
  void  set (int row, int col, string str);
  void  insert (int row, int col, string str);
  void  move_to (int row, int col);
  void  add (int row, int col);
};

typedef struct
{
  string    content;
  string    oldcontent;
} Content;


class  verilogFile:  public  Filehandler
{
private:
  Exec *exec;
  void  send (string msg);
  string  recv ();
  long    se[16];
  bool    comment;
  bool    wire_comment;
  bool    style;
  bool    generate;

public:

  ~
  verilogFile ();
  verilogFile ();
  verilogFile (string path);
  verilogFile (string bin, string path);
  string  getit (string str);
  bool  getcomment ();
  bool  getstyle ();
  void  setregex (string regex);
  void  cout_ (long start, long end);
  void  loadnewfile (string path);
  void  checkVerilogFile (string modname);
  void  checkVerilogFile (string modname, string instname, string defmodname);
  void  modifyInstantiation ();
  void  modifyModuleDefinition ();
  void  change (IOcon con, Type type);
  void  insert (long start, long end, string buffer);
  void  parse ();
};



struct VfilePositions
{
  VfilePositions (int a, int b, int c, int d, int e, int f, bool g):
  module_def (a),
  module_len (b),
  io_split (c),
  io_len (d),
  endmodule (e),
  endmodule_len (f),
  style (g)
  {
  }
  // position of module modulename ( .... 
  int    module_def;
  int    module_len;

  // describes the position where ");" is located which splits the IO definition part from the Instantiation part 
  int    io_split;
  int    io_len;

  // describes the position where endmodule is located
  int    endmodule;
  int    endmodule_len;

  // false -> oldstyle, true -> new style
  bool style;

};

struct Positions
{
  Positions (int a, int b, int c, int d, int e, int f):
  Smpos0 (a),
  Smlen0 (b),
  Smpos1 (c),
  Smlen1 (d),
  Smpos2 (e),
  Smlen2 (f)
  {
  }
  int    Smpos0;
  int    Smlen0;
  int    Smpos1;
  int    Smlen1;
  int    Smpos2;
  int    Smlen2;
};

# endif
