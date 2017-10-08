
# include <string>
# include "Cutter.h"
# include <stack>
# include <vector>

using namespace std;

struct Cons
{
  string     name;
  string     type;
  string     bitwith;

  Cons &operator=( const Cons& cons)
  {
    name = cons.name;
    type = cons.type;
    bitwith = cons.bitwith;
    return *this;
  }

};


enum Type
{
  MODU,
  INST,
  WIRE,
  MODU_REV,
  OPEN_PORT
};

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
  static const    string    MODULE_COMMENT;
  static const    string    INSTAN_COMMENT;
  static const    string    WIRE_COMMENT;
  static const    string    END_COMMENT;
  string    ios;
  string 	  openPortInp;
  string 	  error_msg;
  Cons *   cons;
  // if status is 1 sth. went wrong with the input of the ios
  bool status;
  int    	size;
  bool splitIOinput( string input );

  // helpfunction to split ios by space
  Cons split( string io);

public:
  IOcon (string ios);
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
  string removespaceat( string ios );
  bool  getstatus();
  string  geterrormsg();
  int     getsize ();
  Cons    *getcons ();
  Cutter  getios ();
};


