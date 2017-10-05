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
# include "parser.h"
# include "modhandler.h"

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

class Connection;

struct Parameter
{
  Parameter (int v, int r, string c, map < int, string > *warn, map < int,
	     string > *ill, string fil, string mod, string in):verbose (v),
    restore (r), cfg (c), warning (warn), illegal (ill), files (fil),
    modtxt (mod), input (in)
  {
  };
  Parameter (int v, int r, string cfg):verbose (v), restore (r), cfg (cfg)
  {
  };
  int verbose;			// parameter for more Output
  int restore;
  string cfg;
  map < int, string > *warning;
  map < int, string > *illegal;
  string files;
  string modtxt;
  string input;
  string projectpath;

};


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

/***

	splits a path of a File into directory and filename

***/

class Path
{
private:
  string path;			// Path to the file
  string pathfolder;		// path to the folder which contains the file
  string filename;		// filename
  char delim;			// delimiter 
  void process ();
public:
    Path ();			// Constructor
   ~Path ();			// Destructor
    Path (string path_pa);




    Path (string path_pa, char delim_pa);
  void setpath (string path_pa);	// Will set the path and split it delimiter is '/'
  void setpath (string path_pa, char delim_pa);	// Will set the path and split it by delimiter
  string getpath ();		// returns the path
  string getpathfolder ();	// returns the path to the folder which contains the file
  string getfilename ();	// returns the filename
  bool exists (string path_pa);	// checks if the given path exists
  bool exists ();		// checks if the path set by setpath(path) exists
};

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


/***

	Testcontroller will manipulate the paths, by copy the file of a module to the testfiles folder
	and replaces the path of the Module with the copy.
	The purpose is to modify copys and dont modify the original files.

***/
class TestController
{
public:
  TestController (string modtxt, string csvconn, string cfg);	// Constructor 
  ~TestController ();		// Destructor
  void restoreOriginalFiles (string testfiles);
  void rsyncFiles (Connection * conObj, string testfiles);
  void compileVCS ();
  void clean (string path);

};

class Controller
{

private:

  Modhandler * mod;
  verilogFile *vfile;
  csvFile *csvfile;
  Parser *instance1;
  Parser *instance2;
  Parser *connection;
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

/***

	modifyUnit represents one instance given by a path

***/

class modifyUnit
{
private:
  string instname;		// instancename
  Instance *inst;
  IOcon *con;
  verilogFile *vfile_inst;
  verilogFile *vfile_modu;

public:

    modifyUnit (verilogFile * vfile_inst, verilogFile * vfile_modu,
		Instance * inst, IOcon * con, string instname);
   ~modifyUnit ();
  // adds Ios to module definition
  void addIos ();

  // adds Ios to the module definition, but inverts its type
  void addInvertedIos ();

  //      connects Ios via instantiation
  void connectIos ();

  // connects Instances via wires
  void connectInstances (modifyUnit * u1, modifyUnit * u2);

  // Connects the open Ports
  void handleOpenPorts ();

  // returns the instance
  Instance *getInstance ();

};



/***

	Connection 

***/

class Connection
{

private:

  // gens will save the the instantiations, which are already connected
  map < string, string > gens;

  map < int, modifyUnit * > *con1;
  map < int, modifyUnit * > *con2;

  // contains illegal paths
  map < int, string > warning_paths;

  // contains warning paths
    map < int, string > illegal_paths;

  // represents the Ios we want to add
  IOcon *con_;

  // name of the instance where inst1 and inst2 meet
  string hierarchy;

  // to the first instantiation 
  string inst1;

  // path to the second instantiation
  string inst2;

  // module where instantiation inst1 and inst2 meet
  modifyUnit *bridge;

  // Reference to the "Class" which parses and changes the verilog-files
  verilogFile *vfile;

  // list of paths which we are not allowed to change
  bool illegal_ (map < string, string > *paths, map < int, string > *ill);

  // list of paths we want a notification
  void warning_ (map < string, string > *paths, map < int, string > *warn);

  // checks if the instance was already handled by checking if it exists in the list gens
  bool check_multiple_inst (Instance * instance);

  //help function for the public function *getFileList()
    map < string, string >  *getfilelist (void (*substitude) (Instance *, map < string, string > *));

  bool status;

  // returns the Path to the Instance where inst_1 and inst_2 are both instantiated
  string getsamehierarchy( string inst_1, string inst_2 );

  // Helper for Constructor
  map < int, modifyUnit * >buildConnectiontree( string instance );

public:

  // Old constructor 
    Connection (Modhandler * mod, Parser * con, string inst1, string inst2,
		string python_bin);
  // New constructor
    Connection (Modhandler *mod, Parser *ios, string inst1, string inst2, Parameter param);
  //Destructor
   ~Connection ();

  // connects the instances
  void connect ();

  // process the open ports which are created by modifing the module definition
  void handleOpenPorts ();

  //checks if an path is located in the map illegal_paths and returns true if it exists
  // if a path is located in warning_paths this method will print a warning to the file warning.txt
  bool checkIllegalPaths (map < int, string > *warning_paths, map < int,
			  string > *illegal_paths);

  // returns the bridge
  modifyUnit *getbridge ();

    map < int, modifyUnit * >getCon1 ();
    map < int, modifyUnit * >getCon2 ();

  // returns a list of paths to the files which have open ports
    map < string, string > *getFileListOpenPorts ();

  // returns a list of paths to the files we want to connect
    map < string, string > *getFileList ();

    bool getstatus();

};

# endif
