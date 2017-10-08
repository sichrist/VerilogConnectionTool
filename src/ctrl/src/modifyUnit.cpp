/*
* @Author: simon
* @Date:   2017-09-11 16:27:39
* @Last Modified by:   simon
* @Last Modified time: 2017-09-28 15:07:31
*/


#include "Controller.h"


modifyUnit::modifyUnit (verilogFile * vfile_inst, verilogFile * vfile_modu,
			Instance * inst, IOcon * con, string instname)
{

  this->instname = instname;
  this->inst = inst;
  this->con = con;
  this->vfile_inst = vfile_inst;
  this->vfile_modu = vfile_modu;
}

modifyUnit::~modifyUnit ()
{

}

void
modifyUnit::addIos ()
{

  Module *definition = inst->getdef ();

  vfile_modu->loadnewfile (definition->getpath ());
  vfile_modu->checkVerilogFile (definition->getname ());
  vfile_modu->change (*con, MODU);

}

void
modifyUnit::addInvertedIos ()
{

  Module *definition = inst->getdef ();
  vfile_modu->loadnewfile (definition->getpath ());
  vfile_modu->checkVerilogFile (definition->getname ());
  vfile_modu->change (*con, MODU_REV);


}

void
modifyUnit::connectIos ()
{

  Module *definition = inst->getdef ();
  Module *instantiation = inst->getins ();
  vfile_inst->loadnewfile (instantiation->getpath ());
  vfile_inst->checkVerilogFile (definition->getname (), instname,
        instantiation->getname ());
  vfile_inst->change (*con, INST);



}

void
modifyUnit::handleOpenPorts ()
{

  static int nbr = 0;
  Parser parser;
  string instancename = "";
  string modulename = "";
  string label = "";

  Module *instantiation = inst->getins ();
  Module *definition = inst->getdef ();
  parser.setstring (instname);
  parser.setdelimiter ('.');
  parser.singleparse ();
  //parser.checkforbracket ();

  instancename = parser.getnextbw ();
  inst->getlabel (&label);
  if (label.length () > 0)
    instancename = label + "." + instancename;
  modulename = definition->getname ();

  if (instantiation == NULL)
    cerr << ERROR << "NULL" << RESET << endl;

  vfile_inst->loadnewfile (instantiation->getpath ());
  vfile_inst->checkVerilogFile (definition->getname (), instancename,
        instantiation->getname ());
  vfile_inst->change (*con, OPEN_PORT);
  nbr++;
}

void
modifyUnit::connectInstances (modifyUnit * u1, modifyUnit * u2)
{
  u1->connectIos ();
  u2->connectIos ();
  vfile_inst->change (*con, WIRE);
}

Instance *
modifyUnit::getInstance ()
{
  return this->inst;
}
