/*
* @Author: simon
* @Date:   2017-08-11 14:33:56
* @Last Modified by:   simon
* @Last Modified time: 2017-09-21 10:25:19
*/
# include "modhandler.h"

# include <fstream>
#include <sstream>
#include <string>


Modhandler::~Modhandler ()
{
  map < std::string, Module * >::iterator it;
  if (!modules.empty ())
    for (it = modules.begin (); it != modules.end (); ++it)
      delete it->second;

  map < std::string, Instance * >::iterator iit;
  if (!instances.empty ())
    for (iit = instances.begin (); iit != instances.end (); ++iit)
      delete iit->second;
}

Modhandler::Modhandler (std::string path)
{
  this->path = path;
  load ();
}


void
Modhandler::addModule (Module * mod)
{
  modules[mod->getname ()] = mod;
}

void
Modhandler::addInstance (Instance * inst)
{
  instances[inst->getname ()] = inst;
}

Module *
Modhandler::getModule (std::string name)
{

  return modules[name];
}

Instance *
Modhandler::getInstance (std::string name)
{
  return instances[name];
}

void
Modhandler::printModules ()
{
  map < std::string, Module * >::iterator it;
  cout << "SIZE: " << modules.size () << endl;
  if (modules.empty ())
    return;
  for (it = modules.begin (); it != modules.end (); ++it)
    cout << it->first << " => " << (it->second)->getname () << '\n';
}

void
Modhandler::printInstances ()
{
  map < std::string, Instance * >::iterator it;
  cout << "SIZE: " << instances.size () << endl;
  if (instances.empty ())
    return;
  for (it = instances.begin (); it != instances.end (); ++it)
    cout << (it->second)->getname () << '\n';
}

void
Modhandler::getAllInstantiations (std::string name)
{
  Instance *inst = instances[name];

  if (inst == NULL)
    {
      cout << "INST = NULL " << endl;
      return;
    }

  Module *mod = inst->getdef ();

  if (mod == NULL)
    {
      cout << "INST = NULL " << endl;
      return;
    }

  mod->getAllInstances ();
}

static int
load_helper (std::string * line, std::string cutafter)
{
  std::size_t found;
  std::string tmp;
  if ((found = line->find (cutafter)) != std::string::npos)
    {
      (*line)[line->length () - 1] = '\0';
      tmp = &((*line)[cutafter.length ()]);
      *line = tmp;
      return 1;
    }
  return 0;
}


void
Modhandler::load ()
{
  const std::string module = ":::Module: ";
  const std::string instance = ":::Instance: ";
  const std::string LEVEL = ":::LEVEL: ";
  const std::string POI = ":::Path of instance: ";
  const std::string POM = ":::Path of module: ";
  const std::string IIM = ":::Inst in Module: ";
  const std::string STOP = ":::STOP:::";

  std::string name;
  std::string name_inst;
  std::string instantiation;
  std::string level;
  std::string poI;
  std::string poM;
  std::string iiM;
  std::string input;

  Instance *inst = NULL;
  Module *mod = NULL;
  Module *modInst = NULL;

  if (path.empty ())
    return;

  std::ifstream infile (path.c_str ());

  for (std::string line; getline (infile, line);)
    {
      if (load_helper (&line, module))
	{
	  name = line;
	}
      else if (load_helper (&line, instance))
	{
	  name_inst = line;
	}
      else if (load_helper (&line, IIM))
	{
	  iiM = line;
	}
      else if (load_helper (&line, LEVEL))
	{
	  // we dont need the LEVEL so we do nothing
	}
      else if (load_helper (&line, POI))
	{
	  poI = line;
	}
      else if (load_helper (&line, POM))
	{
	  poM = line;
	}
      else if (load_helper (&line, STOP))
	{
	  if (NULL == (mod = getModule (name)))
	    {
	      mod = new Module ();
	      mod->setname (name);
	      mod->setpath (poM);
	      addModule (mod);
	    }
	  if (NULL == (modInst = getModule (iiM)))
	    {
	      modInst = new Module ();
	      modInst->setname (iiM);
	      modInst->setpath (poI);
	      addModule (modInst);
	    }
	  inst = new Instance ();
	  inst->setname (name_inst);
	  inst->setdef (mod);
	  inst->setins (modInst);
	  inst->setlabel ();
	  mod->addinstance (inst);
	  addInstance (inst);
	}
    }
}
