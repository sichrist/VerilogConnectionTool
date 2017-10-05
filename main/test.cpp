/*
* @Author: simon
* @Date:   2017-08-11 16:05:33
* @Last Modified by:   simon
* @Last Modified time: 2017-09-26 09:10:40
*/

#include <iostream>

using namespace std;


# include <iostream>
# include "parser.h"
# include "modhandler.h"
# include "Filehandler.h"
# include "thread.h"
# include "Controller.h"


using namespace std;

int
main ()
{

/*** ACHTUNG !!!!

	Filehandler.cpp:632

	string tmp_path = this->path;
	this->path = pth;
	pth = tmp_path;
	fullcpy(pth);
ENTFERNEN !!! ***/

  TestController controller
    ("/home/projects/simon/f9/simon_proj/Testprogramm/tests/testfiles/mod.txt",
     "/home/simon/Documents/CPP_TESTS/testfiles/test.csv",
     "/home/simon/Documents/CPP_TESTS/cfg/ict_cfg.cfg");


  /*TestController controller
     ("/home/projects/simon/x1/verification/testprograms/test_ch_enable/sim_model.generic..vcs_rtl_gf40/mod.txt",
     "test.csv", ""); */

  //controller.startRoutine ();


/*
  string bin = "/usr/bin/make";
  string param =
    " -C /home/projects/simon/f9/verification/testprograms/test_dfa_ecc_40_single_sector/";
  string target = " compile";
  char **argv = NULL;
  argv = (char **) calloc (4, sizeof (char *));
  argv[0] = (char *) calloc (bin.length (), sizeof (char));
  argv[1] = (char *) calloc (param.length (), sizeof (char));
  argv[2] = (char *) calloc (target.length (), sizeof (char));
  argv[3] = NULL;

  argv[0] = (char *) (bin.c_str ());
  argv[1] = (char *) (param.c_str ());
  argv[2] = (char *) (target.c_str ());
  cout << argv[0] << argv[1] << argv[2] << endl;
*/

  return 0;
}
