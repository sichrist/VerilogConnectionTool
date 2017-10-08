/*
* @Author: simon
* @Date:   2017-08-14 13:54:40
* @Last Modified by:   simon
* @Last Modified time: 2017-08-28 13:46:32
*/

# ifndef CONTROLLER_H
# define CONTROLLER_H

# include <dirent.h>
# include "Filehandler.h"
# include "Cutter.h"
# include "modhandler.h"
# include "Connection.h"
# include "modifyUnit.h"
# include "Path.h"

# define BOLD     "\033[1m"
# define YELLOW "\x1b[97m"
# define RESET   "\x1b[0m"
# define TRACE  "\x1b[93m"
# define INFO   "\x1b[36m"
# define DEBUG  "\x1b[94m"
# define WARN   "\x1b[95m"
# define ERROR  "\x1b[91m"
# define FATAL  "\x1b[31m"

#define PYTHON_BIN_PATH "py/vparser.py"



typedef struct
{
  Instance *instance;
  string name;
} Pair;

struct csvparse
{
  csvparse (string a, string b, string c, string d):inst1 (a), inst2 (b),
    ios (c), status (d)
  {
  }
  string inst1;
  string inst2;
  string ios;
  string status;
};


class Controller
{

private:

  Modhandler * mod;
  verilogFile *vfile;
  csvFile *csvfile;
  string modtxt;
  string csvConnections;
  string cfg;
  string backupfolder;
  string projectpath;
  Parameter *param;
    map < int, string > *warning;
    map < int, string > *illegal;

  void buildinstancetree (map < int, Pair * >*instances, string instance,
			  string hierarchy);

public:
    map < int, csvparse * >*parse_input (string csvpath);
    Controller (Parameter param);
    Controller (string modtxt, string csvconn, string cfg);
  Connection *buildConnectionObject (string inst1, string inst2, string con,
				     string projpath);
  Connection *startRoutine (int i);
   ~Controller ();
  void start ();
  void startRoutine ();
  void connect (string inst1, string inst2, string con);
  void restoreFiles ();
  void cleanDirs (string path, string pattern);

};



# endif
