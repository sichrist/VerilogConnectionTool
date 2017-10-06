/*
* @Author: simon
* @Date:   2017-08-11 16:05:21
* @Last Modified by:   simon
* @Last Modified time: 2017-08-11 16:17:24
*/
#ifndef MODHANDLER_H
#define MODHANDLER_H

#include <iostream>
#include <string>
#include <map>
#include "module.h"

using namespace std;


/**
	Modhandler stores all instances and Modules given by the mod-file

**/
class Modhandler
{

private:
  string path;			// Path to the mod.txt - file
  map < string, Module * >modules;	// stores all modules
  map < string, Instance * >instances;	// stores all instances
  void load ();

public:
    Modhandler (string path);	// Constructor
   ~Modhandler ();		// Destructor
  void addModule (Module * mod);	// adds a Module to the list modules
  void addInstance (Instance * inst);	// adds a Instance to the list instances
  void printModules ();		// prints all modules to the shell ( debug )
  void printInstances ();	// prints all instances to the shell ( debug )
  Module *getModule (string name);	// returns a pointer to the module named name
  void getAllInstantiations (string name);	// prints all instances of a module named name to the shell
  Instance *getInstance (string name);	// returns a pointer to the instance named name
};


#endif
