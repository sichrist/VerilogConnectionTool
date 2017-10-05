/*
* @Author: simon
* @Date:   2017-08-11 14:34:03
* @Last Modified by:   simon
* @Last Modified time: 2017-09-19 11:05:22
*/
#include "module.h"
#include "parser.h"

#include <iostream>
#include <string>

Module::Module ()
{

}

Module::~Module ()
{

}

void
Module::setname (std::string name)
{
  this->name = name;
}

void
Module::setpath (std::string path)
{
  this->path = path;
}

void
Module::addinstance (Instance * instance)
{
  instances[instance->getname ()] = instance;
}

Instance *
Module::getinstance (std::string name)
{
  return instances[name];
}

map < string, Instance * >*Module::getAllInstances ()
{
  if (instances.empty ())
    return NULL;
  return &instances;
}

std::string Module::getname ()
{
  return this->name;
}

std::string Module::getpath ()
{
  return this->path;
}

void
Instance::setlabel ()
{
  if (name == "")
    {
      label = "";
      return;
    }

  Parser parser;
  parser.setstring (name);
  parser.setdelimiter ('.');
  parser.singleparse ();
  parser.getnextbw ();
  label = parser.getnextbw ();
  std::size_t found = label.find ("[");
  if (found != std::string::npos)
    {
      string tmp = label.substr (0, found);
      label = tmp;
    }
  else
    {
      label = "";
      return;
    }

}
