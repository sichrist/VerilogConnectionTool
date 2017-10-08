# ifndef CONNECTION_H
# define CONNECTION_H


# include "Cutter.h"
# include "Filehandler.h"
# include "modifyUnit.h"
# include "Path.h"
/***

	Connection 

***/


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
  int verbose;      // parameter for more Output
  int restore;
  string cfg;
  map < int, string > *warning;
  map < int, string > *illegal;
  string files;
  string modtxt;
  string input;
  string projectpath;

};


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

  // path to the first instantiation 
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
  map < int, modifyUnit * >buildConnectiontree( Modhandler *modhandler, string instPath );

  modifyUnit *buildBridge( Modhandler *modhandler );

public:
    
  // Constructor
    Connection (Modhandler *mod, string ios, string inst1, string inst2, Parameter param);
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