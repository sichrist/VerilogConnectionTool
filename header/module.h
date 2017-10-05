/*
* @Author: simon
* @Date:   2017-08-11 16:05:15
* @Last Modified by:   simon
* @Last Modified time: 2017-08-21 16:42:46
*/
#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <map>
#include <iostream>

using namespace std;

class Instance;

class Module
{

private:
  string name;			// name of the module
  string path;			// path to the module
    map < string, Instance * >instances;	// list of all instances of this module

public:
    Module ();			// Constructor
   ~Module ();			// Destructor
  void setname (std::string name);	// sets the name
  void setpath (std::string path);	// sets the path
  void addinstance (Instance * instance);	// adds an instance to the map instances
  Instance *getinstance (std::string name);	// returns a reference to the instance given by name, if it doesn't exists, it returns NULL
    map < string, Instance * >*getAllInstances ();	// the list instances
    std::string getname ();	// returns the name
    std::string getpath ();	// returns the path
};


class Instance
{

private:
  string name;			// name of the instance
  string label;			// name of a label if it doesn't exists this string is empty
  Module *definition;		// Module where the instance is defined
  Module *instantiation;	// Module where the instance is instantiated

public:
    Instance ()
  {
    definition = NULL;
    instantiation = NULL;
  };

  ~Instance ()
  {
  };
  void setdef (Module * module)
  {
    definition = module;
  }
  void setins (Module * module)
  {
    instantiation = module;
  }

  bool getlabel (string * str)
  {
    if (label == "")
      return false;
    *str = label;
    return true;
  }

  void setname (string name)
  {
    this->name = name;
  }
  Module *getdef ()
  {
    return this->definition;
  }
  Module *getins ()
  {
    return this->instantiation;
  }
  string getname ()
  {
    return this->name;
  }
  void setlabel ();

};

#endif
