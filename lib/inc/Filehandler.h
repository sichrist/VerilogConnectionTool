
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
# include "parser.h"
# include <sstream>
#include <boost/regex.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include "parser.h"

using namespace
  boost::iostreams;
using namespace
  std;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class
  IOcon;

class
  Filehandler
{
protected:
  ifstream
    read;
  ofstream
    write;
  string
    path;
  long
    start;
  long
    end;

public:
  Filehandler ();
  Filehandler (string path);
  ~
  Filehandler ();
  void
  setpath (string path);
  void
  cpy (string path);
  void
  fullcpy (string path);
  void
  ffwd (string end);

  bool
  seek (string str, string end);
  long
  getstart ();
  long
  getend ();
  string
  getpath ();
};

class
  cfgFile:
  public
  Filehandler
{
  map <
    string,
    map < int,
  string > >
    input;
public:
  void
  parse ();
};

class
  csvFile:
  public
  Filehandler
{
private:

  char
    delimiter;
  long
    pos;

public:

  csvFile ();
  csvFile (string path);
  ~
  csvFile ();
  string
  get (int row, int col);
  void
  setdelimiter (char del);
  void
  set (int row, int col, string str);
  void
  insert (int row, int col, string str);
  void
  move_to (int row, int col);
  void
  add (int row, int col);
};

enum Type
{
  MODU,
  INST,
  WIRE,
  MODU_REV,
  OPEN_PORT
};

typedef struct
{
  string
    content;
  string
    oldcontent;
} Content;


class
  verilogFile:
  public
  Filehandler
{
private:
  Exec *
    exec;
  string
    regex;
  void
  send (string msg);
  string
  recv ();
  long
    se[16];
  bool
    comment;
  bool
    wire_comment;
  bool
    style;
  bool
    generate;

public:

  ~
  verilogFile ();
  verilogFile ();
  verilogFile (string path);
  verilogFile (string bin, string path);
  string
  getit (string str);
  bool
  getcomment ();
  bool
  getstyle ();
  void
  setregex (string regex);
  void
  cout_ (long start, long end);
  void
  loadnewfile (string path);
  void
  checkVerilogFile (string modname);
  void
  checkVerilogFile (string modname, string instname, string defmodname);
  void
  modifyInstantiation ();
  void
  modifyModuleDefinition ();
  void
  change (IOcon con, Type type);
  void
  insert (long start, long end, string buffer);
  void
  parse ();
};


typedef struct
{
  string *
    name;
  string *
    type;
  string *
    bitwith;
} Cons;


enum ConType
{
  module,
  instantiation,
  wire,
  modulerev
};

/***

	cons[0]  -> type
			 -> bitwith
			 -> name
	cons[1]	 -> ....
		.
		.
		.
	cones[size-1] ...

***/

class
  IOcon
{

private:
  static const
    string
    MODULE_COMMENT;
  static const
    string
    INSTAN_COMMENT;
  static const
    string
    WIRE_COMMENT;
  static const
    string
    END_COMMENT;
  Parser    ios;
  string    name;
  string 	openPortInp;
  string    def;
  string    defrev;
  string    ins;
  string    expr_name;
  string 	error_msg;
  Cons *    cons;
  // if status is 1 sth. went wrong with the input of the ios
  bool status;
  int    	size;
  bool splitIOinput( string input );

public:
  IOcon (Parser ios);
  ~IOcon ();
  void  parse ();
  string  getDef ();
  string  getIns ();
  string  getDefrev ();
  string  getname ();

  string  getModComment ();
  string  getInsComment ();
  string  getWirComment ();
  string  getEndComment ();

  string  buildNormal (Type type);
  string  buildOld (bool section, Type type);
  string  buildWire ();

  bool  getstatus();
  string  geterrormsg();
  int     getsize ();
  Cons    *getcons ();
  Parser  getios ();
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
  int
    module_def;
  int
    module_len;
  // describes the position where ");" is located which splits the IO definition part from the Instantiation part 
  int
    io_split;
  int
    io_len;
  // describes the position where endmodule is located
  int
    endmodule;
  int
    endmodule_len;

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
  int
    Smpos0;
  int
    Smlen0;
  int
    Smpos1;
  int
    Smlen1;
  int
    Smpos2;
  int
    Smlen2;
};

// NOT USED!!!
class
  vfile
{
private:
  string
    path;
  boost::regex
    module_expr;
  boost::regex
    iodefs_expr;
  boost::regex
    endmod_expr;
  boost::regex
    instan_expr;
  string
    fileContent;
  mapped_file_source
    file;
  map <
    string,
  VfilePositions * >
    vfileStats;
  map < int,
  Positions * >
  parseExpression (string expr_str);
  int
  getCommentPos (int start, int len, string type);
  bool
  Old_New_style (string str);
  void
  insert_inst (string module_def, string modulename, string name, IOcon ios,
	       Type type);
  bool
  checkIos (int start, int len, IOcon ios);

public:
  vfile (string path);
  ~
  vfile ();
  void
  insert_ios (string modulename, IOcon ios);
  void
  connect_inst (string module_def, string modulename, string name, IOcon ios);
  void
  connect_openPorts (string module_def, string modulename, string name,
		     IOcon ios);
  void
  insert_wire (string modulename, IOcon ios);
};


# endif
