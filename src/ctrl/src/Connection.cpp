/*
* @Author: simon
* @Date:   2017-09-11 16:26:27
* @Last Modified by:   simon
* @Last Modified time: 2017-09-29 16:05:04
*/

#include "Controller.h"
#include <boost/regex.hpp>


inline void
filelist (Instance * instance, map < string, string > *paths)
{
  string def_path = "";
  string ins_path = "";

  Module *mod = NULL;
  mod = instance->getdef ();
  if (mod == NULL)
    cerr << "filelist(): " << instance->getname () << " Not found" << endl;

  def_path = mod->getpath ();
  mod = instance->getins ();
  ins_path = mod->getpath ();

  Path pathObject(def_path);
  
  (*paths)[pathObject.getfilename ()] = pathObject.getfolder ();

  pathObject.set (ins_path);
  (*paths)[pathObject.getfilename ()] = pathObject.getfolder ();
}


inline void
filelistOpenPorts (Instance * instance, map < string, string > *paths)
{
  Module *mod = NULL;
  mod = instance->getdef ();
  map < string, Instance * >*instances = NULL;
  Path pathObject;
  instances = mod->getAllInstances ();
  if (instances == NULL)
    {
      cerr << "No further instances of: " << mod->getname () << endl;
      return;
    }

  for (map < string, Instance * >::iterator it = instances->begin ();
       it != instances->end (); ++it)
    {
      if (it->first.compare (instance->getname ()) == 0)
	{
	  cerr << instance->getname () << " nicht bearbeiten!" << endl;
	  continue;
	}
      cerr << "[" << it->second->getname () << "]" << endl;
      pathObject.set (it->second->getins ()->getpath ());

      (*paths)[pathObject.getfilename ()] = pathObject.getfolder ();

    }


}

inline string Connection::getsamehierarchy( string inst_1, string inst_2 )
{

	Cutter insPath_1(inst_1,'.');
	Cutter insPath_2(inst_2,'.');
	string ret;
   	string tmp ,tmp_1, tmp_2;


   	insPath_1.begin();
   	insPath_2.begin();

	while( insPath_1.fin() || insPath_2.fin() )
	{
		tmp_1 = insPath_1.next();
		tmp_2 = insPath_2.next();
		if( tmp_1 == tmp_2 )
		{
			tmp += tmp_1 + ".";
		}
	}

	if( tmp[tmp.size()-1] == '.')
		ret = tmp.substr(0,tmp.size()-2);
	else
	{
		ret = tmp;
	}


  return ret;

}

inline modifyUnit *Connection::buildBridge( Modhandler *modhandler )
{
	Instance *instance = NULL;
	string instname;

	Cutter cutter(hierarchy,'.');
	cutter.end();

	instname = cutter.next();
	instance = modhandler->getInstance(hierarchy);

	return new modifyUnit(vfile, vfile, instance, con_, instname );

}

inline  map < int, modifyUnit * > *Connection::buildConnectiontree( Modhandler *modhandler, string instPath )
{

	string instpath 					= ""	;
	string instname 					= ""	;
	map < int, modifyUnit * > *retmap 	= NULL	;
	Instance *instance  				= NULL	;

	Cutter insPath(instPath,'.');
	Cutter insName(instPath,'.');

	retmap = new map < int, modifyUnit * >;

	insPath.begin();
	insName.end();

	for (int i = 0; (instpath = insPath.next ()) != hierarchy; i++)
	{

		if( instpath == "" )
			break;
		instance 		= 	modhandler->getInstance( instpath );
		instname 		= insName.next();
		(*retmap)[i]	= 	new modifyUnit (vfile, vfile, instance, con_, instname);
	}


	return retmap;
}


/*
Connection::Connection (Modhandler * mod, Parser * con, string inst1,
      string inst2, string python_bin)
      string inst2, string python_bin)
*/

// Builds the Connection Object

Connection::Connection (Modhandler *mod, string ios, string inst1, string inst2 , Parameter param)
{

    // neutralize

	vfile               = NULL	;
	bridge              = NULL	;
	con_                = NULL	;
	status 				= 0 	;

	// initialize

	vfile 		= new verilogFile( param.projectpath + PYTHON_BIN_PATH, "" );
	con_ 		= new IOcon( ios );

	// check if building the IOs was successfull

	if( con_->getstatus() )
		goto error;

	hierarchy 	= getsamehierarchy( inst1, inst2 );
	con1 		= buildConnectiontree( mod, inst1 );
	con2 		= buildConnectiontree( mod, inst2 );

	bridge 		= buildBridge( mod );



/*
  Parser instparser1;
  Parser instparser2;
  Parser instnameparser;
  Instance *instance = NULL;
  vfile = NULL;
  bridge = NULL;
  connection_ = NULL;

  connection_ = con;
  con_ = NULL;
 
  con_ = new IOcon (*con);

  string tmp_instname;
  ofstream write;
  status = 0;

  if( con_->getstatus() )
    goto error;

  instparser1.setstring (inst1);
  instparser1.setdelimiter ('.');
  instparser1.parse ();
  instparser1.checkforbracket ();
  instparser2.setstring (inst2);
  instparser2.setdelimiter ('.');
  instparser2.parse ();
  instparser2.checkforbracket ();
  vfile = new verilogFile (python_bin, "");

  hierarchy = instparser1.getsamehier (&instparser2);

  instnameparser.setstring (inst1);
  instnameparser.setdelimiter ('.');
  instnameparser.singleparse ();

  for (int i = 0; (tmp_instname = instparser1.getnext ()) != hierarchy; i++)
    {
      if (tmp_instname == "")
	break;
      instance = mod->getInstance (tmp_instname);
      if (instance == NULL)
	{
	  cerr << "No instance with name: " << tmp_instname << " found" <<
	    endl;
	  exit (-1);
	}
      con1[i] =
	new modifyUnit (vfile, vfile,
			mod->getInstance (tmp_instname), con_,
			instnameparser.getnextbw ());
    }

  instnameparser.setstring (inst2);
  instnameparser.setdelimiter ('.');
  instnameparser.singleparse ();

  for (int i = 0; (tmp_instname = instparser2.getnext ()) != hierarchy; i++)
    {
      if (tmp_instname == "")
	break;
      instance = mod->getInstance (tmp_instname);
      if (instance == NULL)
	{
	  cerr << "No instance with name: " << tmp_instname << " found" <<
	    endl;
	  exit (-1);
	}
      con2[i] =
	new modifyUnit (vfile, vfile, instance, con_,
			instnameparser.getnextbw ());
    }
  tmp_instname = instnameparser.getnextbw ();
  bridge =
    new modifyUnit (vfile, vfile, mod->getInstance (hierarchy), con_,
		    tmp_instname);


  if (!exists_file ("/home/simon/Documents/CPP_TESTS/warning.txt"))
    write.open ("/home/simon/Documents/CPP_TESTS/warning.txt",
		fstream::trunc);
  else
    write.open ("/home/simon/Documents/CPP_TESTS/warning.txt", fstream::app);
  write.close ();

  write.open ("/home/simon/Documents/CPP_TESTS/warning.txt", fstream::app);
  write <<
    "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  write <<
    "+                             Warning                               +\n";
  write <<
    "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
  write << "\t" << inst1 << endl << "\t" << inst2 << endl;
  write <<
    "---------------------------------------------------------------------\n\n";
  write.close ();

  // start python regex process
  vfile->parse ();

 */

  return;

  error:
   cerr << con_->geterrormsg()<<endl;
     delete con_;
     delete vfile;
     con_ = NULL;
     vfile = NULL;
    status = 1;
}

Connection::~Connection ()
{
 /* if (con_ != NULL)
    delete con_;
*/
  for (unsigned int i = 0; i < con1->size (); i++)
    delete (*con1)[i];
  for (unsigned int i = 0; i < con2->size (); i++)
    delete (*con2)[i];
  if (bridge != NULL)
    delete bridge;
  if (vfile != NULL)
    delete vfile;

  vfile = NULL;
  bridge = NULL;
}



inline bool
check_ill (string str)
{
  return true;
}

inline bool
handle_warn (string str)
{
  ofstream write;

  if (!exists_file ("/home/simon/Documents/CPP_TESTS/warning.txt"))
    write.open ("/home/simon/Documents/CPP_TESTS/warning.txt",
		fstream::trunc);
  else
    write.open ("/home/simon/Documents/CPP_TESTS/warning.txt", fstream::app);

  write << str << endl;
  write.close ();
  return false;
}

inline bool
loop_through (map < string, string > *paths, map < int, string > *list,
	      bool (*func) (string str))
{

  for (map < string, string >::iterator it = paths->begin ();
       it != paths->end (); ++it)
    {
      for (map < int, string >::iterator it2 = list->begin ();
	   it2 != list->end (); ++it2)
	{
	  if (it->second.find (it2->second) != std::string::npos)
	    {
	      if (func (it->second + "/" + it->first))
		return true;
	    }
	}
    }
  return false;
}

inline bool
Connection::illegal_ (map < string, string > *paths, map < int, string > *ill)
{
  return loop_through (paths, ill, check_ill);
}

inline void
Connection::warning_ (map < string, string > *paths, map < int,
		      string > *warn)
{
  loop_through (paths, warn, handle_warn);
}


enum State
{ none, warning, illegal };

bool
  Connection::checkIllegalPaths (map < int, string > *warning_paths,
				 map < int, string > *illegal_paths)
{

  cout << "ADRESSE: "<< illegal_paths << " - "<<warning_paths<<endl;
  for (unsigned int i = 0; i < warning_paths->size (); i++)
    cout << (*warning_paths)[i] << endl;
 

  map < string, string > *regular_paths = NULL;
  map < string, string > *openPort_paths = NULL;
  bool ret = false;
/*
  if (path == "")
    return false;
  static State state = none;
  static map < int, string > warning_paths;
  static map < int, string > illegal_paths;
  static bool read_ = false;

  ifstream read;
  read.open (path);
  string cont = "";
  int i_warn = 0;
  int i_erro = 0;

  if (!read_)
    {
      while (!read.eof ())
	{
	  read >> cont;
	  if (cont.compare ("[WARNING]:") == 0)
	    state = warning;
	  else if (cont.compare ("[ILLEGAL]:") == 0)
	    state = illegal;
	  if (!exists_dir (cont))
	    continue;
	  if (state == warning)
	    warning_paths[i_warn++] = cont;

	  if (state == illegal)
	    illegal_paths[i_erro++] = cont;

	  cont = "";
	}
      read_ = true;
    }
*/

  regular_paths = getFileList ();
  openPort_paths = getFileListOpenPorts ();
  warning_ (regular_paths, warning_paths);
  warning_ (openPort_paths, warning_paths);
  ret = (illegal_ (regular_paths, illegal_paths)
	 || illegal_ (openPort_paths, illegal_paths));
  return ret;
}



inline
  map <  string,  string >  *Connection::getfilelist (void (*substitute)(Instance *, map < string, string > *))
{

	map < string, string > *paths = new map < string, string >;
	map < int,  modifyUnit * > *con = con1;

	string modpath = "";

	for (int j = 0; j < 2; j++)
	{
		for (unsigned int i = 0; i < con->size (); i++)
		{
			substitute ((*con)[i]->getInstance (), paths);
		}

		con = con2;
    }

	return paths;
}

map < string, string > *Connection::getFileList ()
{
  return this->getfilelist (filelist);
}

map < string, string > *Connection::getFileListOpenPorts ()
{
  return this->getfilelist (filelistOpenPorts);
}


/* 
  
  This function makes sure that we do not add connections to instances more than once.


	case1:

	takes a instance and checks if it instance name contains "[0]" if yes it will store 
	it in the map ret and returns true.
	The next time if an instance name contains a [i] with i >= 0 then it will return false,
	so we know that we have already handled this instance.

	case2:

	The instance is no auto generated instance. 


*/
bool
Connection::check_multiple_inst (Instance * instance)
{

  string name = instance->getname ();
  Module *mod = instance->getins ();
  string modname = mod->getname ();
  string gen;
  string instname;
  
  boost::match_results < string::const_iterator > what;
  boost::regex generate_in_name_expr
  {
  "(\\w+)\\[(\\d+)\\]\\.(.*)?"};
  string genvarnbr = "";
  std::pair < std::map < string, string >::iterator, bool > ret;

  if (regex_search (name, what, generate_in_name_expr))
    {
      genvarnbr = what[2];
      instname = what[3];
      while (regex_search (instname, what, generate_in_name_expr))
	{
	  genvarnbr = what[2];
	  instname = what[3];
	}
      instance->getlabel (&gen);
      
      Path path(instname, '.');
      instname = path.getfilename ();
      ret =
  gens.insert (std::pair < string,
         string > (modname + gen + instname, " "));
    }
  else
    {

      Path path(name, '.');
      ret = gens.insert (std::pair < string, string > (modname + path.getfilename (), " "));

    }

  return ret.second;
}

void
Connection::handleOpenPorts ()
{

	modifyUnit *modify_instance = NULL;
	Module *mod = NULL;
	Instance *instance = NULL;
	map < string, Instance * >*instances;
	string instname;
	string removeMe = "";
	map < int, modifyUnit * >*con = con1;

	for (int j = 0; j < 2; j++)
	{

		gens.clear ();

		for (unsigned int i = 0; i < con->size (); i++)
		{
			modify_instance = (*con)[i];
			instance = modify_instance->getInstance ();
			instname = instance->getname ();
			mod = instance->getdef ();
			instances = mod->getAllInstances ();

			if (instances == NULL)
			{
				cerr << "No further instances of: " << mod->getname () << endl;
				continue;
			}

			// make sure the Instance in *con[i] is already in the gens map
			check_multiple_inst (instance);

			for (map < string, Instance * >::iterator it = instances->begin ();it != instances->end (); ++it)
			{

				cout << "[" << it->first << "] == [" << instname << "]" << endl;

				if (!check_multiple_inst (it->second))
				{
					it->second->getlabel (&removeMe);
					cout << "Multiple : continue" << removeMe << it->second->getname () << endl;
					continue;
				}
				if (it->first.compare (instname) == 0)
				{
					continue;
				}
	  

				modify_instance = new modifyUnit (vfile, vfile, it->second, con_,(it->second)->getname ());
				modify_instance->handleOpenPorts ();

			}
		}
		con = con2;
    }
}

void Connection::connect ()
{

	int size = con1->size ();

	for (int i = 0; i < size; i++)
		(*con1)[i]->addIos ();
	for (int i = 0; i < size - 1; i++)
		(*con1)[i]->connectIos ();

	size = con2->size ();

	for (int i = 0; i < size; i++)
		(*con2)[i]->addInvertedIos ();
	for (int i = 0; i < size - 1; i++)
		(*con2)[i]->connectIos ();

	if (con2->size () == 0)
	{
		(*con1)[con1->size () - 1]->connectIos ();
		bridge->addIos ();
	}
	else if (con1->size () == 0)
	{
		(*con2)[con2->size () - 1]->connectIos ();
		bridge->addInvertedIos ();
	}
	else
	{
		bridge->connectInstances ( (*con2)[con2->size () - 1],(*con1)[con1->size () - 1]);
	}
}

map < int, modifyUnit * >*Connection::getCon1 ()
{
  return con1;
}

map < int, modifyUnit * >*Connection::getCon2 ()
{
  return con2;
}

modifyUnit *Connection::getbridge ()
{
  return bridge;
}

bool Connection::getstatus()
{
  return status;
}