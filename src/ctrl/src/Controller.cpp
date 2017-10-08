/*
* @Author: simon
* @Date:   2017-08-14 13:55:01
* @Last Modified by:   simon
* @Last Modified time: 2017-09-29 14:31:49
*/

# include "Controller.h"

// saves file we want to change in path dstfiles
void
rsync (map < string, string > *filelist, string dstfiles)
{

	ofstream write;
	
	//Path pathObject;
	string arg = "";
	
	static string filelisttxt = "filelist.txt";

	if (!exists_file (dstfiles + filelisttxt))
		write.open ((dstfiles + filelisttxt).c_str (), fstream::trunc);
	else
		write.open ((dstfiles + filelisttxt).c_str (), fstream::app);

	for (std::map < string, string >::iterator it = filelist->begin (); it != filelist->end (); ++it)
    {
		if (exists_file (dstfiles + it->first))
			continue;
		arg += it->second + "/" + it->first + " ";
		write << it->first + "\t" + it->second + "/" << endl;
    }
	
	cout << "RSYNC + " << arg << " + " << filelist->size () << endl;
	if (arg.length () < 1)
		return;
 	
 	string cmd = "rsync -v " + arg + " " + dstfiles;
	system ((const char *) cmd.c_str ());

}

void
Controller::cleanDirs (string path, string pattern)
{
	string cmd = "rm -f " + path + pattern;
	cout << cmd << endl;
	system ((const char *) cmd.c_str ());
}

void
Controller::restoreFiles ()
{
	
	ifstream read;
	static string filelisttxt = "filelist.txt";
	read.open (backupfolder + "filelist.txt");
	string file;
	string path;
	map < string, string > restore;
	string cmd = "rsync ";
	string src = "";
	string dst = "";
	string buffer = "";

	while (!read.eof ())
	{
		read >> file;
		read >> path;
		if (file == "" || path == "")
			break;

		restore[path] += backupfolder + file + " ";
		path = "";
		file = "";
    }

	for (std::map < string, string >::iterator it = restore.begin ();it != restore.end (); ++it)
    {
		src = it->second;
		dst = it->first;
		buffer = cmd + src + dst;
		system ((const char *) buffer.c_str ());
    }
	
	read.close ();
/*	ofstream write;
	write.open (testfiles + "filelist.txt", std::ofstream::trunc);
	write.close ();*/
}

Controller::Controller (Parameter param)
{
  
	this->modtxt = param.modtxt;
	this->csvConnections = param.input;
	this->instance1 = new Parser ();
	this->instance2 = new Parser ();
	this->connection = new Parser ();
	this->instance1->setdelimiter ('.');
	this->instance2->setdelimiter ('.');
	this->connection->setdelimiter ('|');
	this->mod = new Modhandler (modtxt);
	this->cfg = cfg;
	this->warning= param.warning;
	this->illegal= param.illegal;
	this->vfile = new verilogFile (param.projectpath + "py/vparser.py", "");
	this->csvfile = NULL;
	this->backupfolder = param.files;
	this->param = &param;
	this->projectpath = param.projectpath;
}

Controller::Controller (string modtxt, string csvconn, string cfg)
{
	this->modtxt = modtxt;
	this->csvConnections = csvconn;
	this->instance1 = new Parser ();
	this->instance2 = new Parser ();
	this->connection = new Parser ();
	this->instance1->setdelimiter ('.');
	this->instance2->setdelimiter ('.');
	this->connection->setdelimiter ('|');
	this->mod = new Modhandler (modtxt);
	this->cfg = cfg;
	this->vfile = new verilogFile ();
	this->csvfile = NULL;
}

map < int, csvparse * >*
Controller::parse_input (string csvpath)
{
	string fileContent;
	mapped_file_source file;

	file = mapped_file_source (csvpath);
	fileContent = string (file.data (), file.size ());

	map < int, csvparse * >*ret = new map < int, csvparse * >;

	string::const_iterator begin = fileContent.begin ();
	string::const_iterator end = fileContent.end ();
	boost::match_results < string::const_iterator > what;

	boost::regex csvexpr{"(\\S+)\t(\\S+)\t(.*?)\t+(\\S*)\t*\n"};

	for (int i = 0; regex_search (begin, end, what, csvexpr); i++)
	{
		csvparse *tmp = new csvparse (what[1], what[2], what[3], what[4]);
		(*ret)[i] = tmp;
    
		begin = what[0].second;
    }
	
	file.close ();
	
	return ret;
}

Controller::~Controller ()
{
	delete this->instance1;
	delete this->instance2;
	delete this->connection;
	delete this->mod;
	if (vfile != NULL)
		delete vfile;
	if (csvfile != NULL)
		delete this->csvfile;
}

Connection *
Controller::buildConnectionObject (string inst1, string inst2, string con,string projpath)
{

	Parser singleparse1;
	Parser singleparse2;

	instance1->setstring (inst1);
	instance1->parse ();
	instance1->checkforbracket ();
	instance2->setstring (inst2);
	instance2->parse ();
	instance2->checkforbracket ();

	connection->setstring (con);
	connection->singleparse ();

	instance1->parse ();
	instance2->parse ();
	
	string shr = instance1->getsamehier (instance2);

	if (shr.compare ("") == 0)
	{

		cout << inst1 << "\t" << inst2 << endl;
		cerr << "!NO COMMON MODULE " << endl;
		cout << "!NO COMMON MODULE " << endl;
		return NULL;
	}


	Connection *connection_ = new Connection (mod, connection, inst1, inst2, projectpath +"py/vparser.py");

  
	if( connection_->getstatus() )
	{
		delete connection_;
		connection_ = NULL;
	}

	return connection_;

}

void Controller::start ()
{

	Connection *conObj = NULL;
	map < string, string > *rsyncfiles = NULL;
	map < int, csvparse * >*input = parse_input (csvConnections);

	for (unsigned int i = 0; i < input->size (); i++)
	{
		cout << "1:\t" << (*input)[i]->inst1 << endl
		<< "2:\t" << (*input)[i]->inst2 << endl
		<< "3:\t" << (*input)[i]->ios << endl << "4:\t" << (*input)[i]->
		status << endl;
		cout << "---------\n\n\n";

		if ((*input)[i]->status == "CONNECTED")
			continue;


		cout << "!!!projectpath: "<<projectpath << endl;
		conObj =	buildConnectionObject ((*input)[i]->inst1, (*input)[i]->inst2,(*input)[i]->ios, projectpath);

	if (conObj == NULL)
	{
		cerr << "!!!Couldnt build an connection object of the instances: \n\n\n" +
		(*input)[i]->inst1 + "\t" + (*input)[i]->inst2 << endl;
		continue;
	}

		cout << "ADRESSE: "<< illegal << " - "<<warning<<endl;
		if (conObj->checkIllegalPaths (warning, illegal ) )
		{
			cerr << "CONNECTION : "+(*input)[i]->inst1 + "\t" + (*input)[i]->inst2+ " is illegal"<<endl;
			delete conObj;
			continue;
		}
    

      rsyncfiles = conObj->getFileListOpenPorts ();
      rsync (rsyncfiles, backupfolder);
      delete rsyncfiles;

      rsyncfiles = conObj->getFileList ();
      rsync (rsyncfiles, backupfolder);
      delete rsyncfiles;

      conObj->connect ();
      conObj->handleOpenPorts ();
      delete conObj;
      conObj = NULL;

    }

  for (unsigned int i = 0; i < input->size (); i++)
    delete (*input)[i];
  delete input;
}

