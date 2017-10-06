/*
* @Author: simon
* @Date:   2017-09-11 16:40:30
* @Last Modified by:   simon
* @Last Modified time: 2017-09-26 12:01:52
*/


# include "Controller.h"

# define BOLD     "\033[1m"
# define YELLOW "\x1b[97m"
# define RESET   "\x1b[0m"
# define TRACE  "\x1b[93m"
# define INFO   "\x1b[36m"
# define DEBUG  "\x1b[94m"
# define WARN   "\x1b[95m"
# define ERROR  "\x1b[91m"
# define FATAL  "\x1b[31m"

void
rsyncConObjs (map < string, string > *filelist, string dstfiles)
{

/*	ofstream write;
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

	if (arg.length () < 1)
		return;
	string cmd = "rsync -v " + arg + " " + dstfiles;
	system ((const char *) cmd.c_str ());*/
}

void
print_instantiations (Connection * conObj)
{
  cout << WARN;
  Instance *instance = NULL;
  Module *mod = NULL;
  map < int, modifyUnit * >con = conObj->getCon1 ();
  for (int j = 0; j < 2; j++)
    {

      for (unsigned int i = 0; i < con.size (); i++)
	{

	  instance = con[i]->getInstance ();
	  if (instance == NULL)
	    continue;
	  mod = instance->getdef ();
	  mod->getAllInstances ();

	}
      con = conObj->getCon2 ();
    }

  cout << RESET;

}

map < int, csvparse * >*
parse_input (string csvpath)
{
  string fileContent;
  mapped_file_source file;

  file = mapped_file_source (csvpath);
  fileContent = string (file.data (), file.size ());

  map < int, csvparse * >*ret = new map < int, csvparse * >;

  string::const_iterator begin = fileContent.begin ();
  string::const_iterator end = fileContent.end ();
  boost::match_results < string::const_iterator > what;
  boost::regex csvexpr
  {
  "(\\S+)\t(\\S+)\t(.*?)\t+(\\S*)\t*\n"};

  for (int i = 0; regex_search (begin, end, what, csvexpr); i++)
    {
      csvparse *tmp = new csvparse (what[1], what[2], what[3], what[4]);
      (*ret)[i] = tmp;
      cerr << what[0] << endl;
      cerr << what[1] << "\n" << what[2] << "\n" << what[3] << "\n" << what[4]
	<< endl;
      cerr << endl;
      begin = what[0].second;
    }
  file.close ();
  return ret;
}


TestController::TestController (string modtxt, string csvconn, string cfg)
{

  map < int, csvparse * >*ret = NULL;
  ret = parse_input (csvconn);
  if (ret->size () == 0)
    {
      cerr << " Couldn't parse " << csvconn << endl;
      return;
    }

  for (std::map < int, csvparse * >::iterator it = ret->begin ();
       it != ret->end (); ++it)
    {
      cout << it->first
	<< "\n\t" << it->second->inst1
	<< "\n\t" << it->second->inst2
	<< "\n\t" << it->second->ios << "\n\t" << it->second->status << endl;
    }

  ofstream write;
  if (!exists_file ("/home/simon/Documents/CPP_TESTS/warning.txt"))
    write.open ("/home/simon/Documents/CPP_TESTS/warning.txt",
		fstream::trunc);
  else
    write.open ("/home/simon/Documents/CPP_TESTS/warning.txt", fstream::app);
  write.close ();


  string testfiles = "/home/simon/Documents/CPP_TESTS/testfiles/";
  Controller controller (modtxt, csvconn, cfg);
  map < string, string > *rsyncfiles = NULL;

  restoreOriginalFiles (testfiles);
  return;

  for (int i = 0; i < 11; i++)
    {
      Connection *conObj = controller.startRoutine (i);

      if (conObj == NULL)
	continue;

//                        print_instantiations (conObj);

      /*     if (conObj->checkIllegalPaths
         ("/home/simon/Documents/CPP_TESTS/cfg/paths.cfg"))
         {
         delete conObj;
         continue;
         } */
      rsyncfiles = conObj->getFileListOpenPorts ();
      rsyncConObjs (rsyncfiles, testfiles);
      rsyncfiles = conObj->getFileList ();
      rsyncConObjs (rsyncfiles, testfiles);

      conObj->connect ();
      conObj->handleOpenPorts ();

      delete conObj;
      conObj = NULL;




    }

  //compileVCS ();
  //restoreOriginalFiles (testfiles);
  cout << WARN << "ENDE" << RESET << endl;

}

TestController::~TestController ()
{
  //clean ("/home/simon/Documents/CPP_TESTS/testfiles/");
  //clean ("/home/simon/Documents/CPP_TESTS/bin/");
}

void
TestController::clean (string path)
{
  string cmd = "rm -f " + path + "*.v*";
  system ((const char *) cmd.c_str ());
}

void
TestController::restoreOriginalFiles (string testfiles)
{
  ifstream read;
  read.open (testfiles + "filelist.txt");
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

      restore[path] += testfiles + file + " ";
      path = "";
      file = "";
    }

  for (std::map < string, string >::iterator it = restore.begin ();
       it != restore.end (); ++it)
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

void
TestController::compileVCS ()
{


  pid_t child;

  char cmd1[] = "make";
  char *args1[] = {
    (char *) "make", (char *) "-C",
    (char *)
      "/home/projects/simon/f9/verification/testprograms/test_dfa_ecc_40_single_sector/",
    (char *) "compile", NULL
      //(char *) "sim_gui", NULL
  };

  char *args2[] = {
    (char *) "make",
    (char *) "-C",
    (char *)
      "/home/projects/simon/f9/verification/testprograms/test_dfa_ecc_40_single_sector/",
    (char *) "clean", NULL
  };

  child = fork ();
  if (child == 0)		// child process
    {
      //execv (bin.c_str (), argv);
      execvp (cmd1, args1);
      perror ("perror: Exec");
    }

  int status;
  if (-1 == waitpid (child, &status, 0))
    {
      if (!WIFEXITED (status) || WEXITSTATUS (status) != 0)
	{
	  cerr << "Process " << child << " (pid " << child << ") failed" <<
	    endl;
	}
    }

  child = fork ();
  if (child == 0)		// child process
    {
      //execv (bin.c_str (), argv);
      execvp (cmd1, args2);
      perror ("perror: Exec");
    }

  if (-1 == waitpid (child, &status, 0))
    {
      if (!WIFEXITED (status) || WEXITSTATUS (status) != 0)
	{
	  cerr << "Process " << child << " (pid " << child << ") failed" <<
	    endl;
	}
    }

}
