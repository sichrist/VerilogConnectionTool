# ifndef MODIFYUNIT_H
# define MODIFYUNIT_H

# include <iostream>
# include <string>
# include "IOcon.h"
# include "Filehandler.h"
# include "module.h"

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

# endif