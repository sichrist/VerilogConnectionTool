/*
* @Author: simon
* @Date:   2017-09-25 13:59:38
* @Last Modified by:   simon
* @Last Modified time: 2017-09-29 14:29:25
*/



# include <iostream>
# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>
# include "parser.h"
# include "modhandler.h"
# include "Filehandler.h"
# include "thread.h"
# include "Controller.h"



using namespace std;

void print_param( Parameter param )
{
	string W = "";
	string I = "";
	Path path;

	for (map < int, string >::iterator it = (param.warning)->begin ();
				 it != (param.warning)->end (); ++it)
		W += "\""+it->second +"\"\n";
	for (map < int, string >::iterator it = (param.illegal)->begin ();
				 it != (param.illegal)->end (); ++it)
		I += "\""+it->second+"\"\n";
	cout <<"---------------PARAMETER--------------------"<<endl;
	cout 	<< "verbose:\t \""<< param.verbose 	<<"\"\n"
	 		<< "cfg:    \t \""<< param.cfg 		<<"\"\n"
	 		<< "warning:\t \n"<< W 	<<"\n"
	 		<< "illegal:\t \n"<< I 	<<"\n"
	 		<< "files:  \t \""<< param.files 	<<"\"\n"
	 		<< "modtxt: \t \""<< param.modtxt 	<<"\"\n"
			<< "input:  \t \""<< param.input 	<<"\"\n"
			<< "Projectpath\t \""<< param.projectpath <<"\""<<endl;

	cout <<"-------------------------------------------"<<endl;
}

void fill_param( Parameter *par )
{
	string fileContent;
	string error_msg = "Ups! parsing "+par->cfg+" failed\n";
	mapped_file_source file;
	file = mapped_file_source (par->cfg);
	fileContent = string (file.data (), file.size ());
	string warning = "";
	string illegal = "";
	par->illegal = new map<int, string>;
	par->warning = new map<int, string>;
	
	
	boost::match_results < string::const_iterator > what;
	boost::regex modtxt_expr	{"\\[MODTXT\\]:\\s*(\\S*)[\\s\\S]*"};
	boost::regex input_expr		{"\\[INPUT\\]:\\s*(\\S*)[\\s\\S]*"};
	boost::regex warning_expr	{"\\[WARNING\\]\\s*\\{\\s*((\\s*|\\S*)*)\\}"};
	boost::regex illegal_expr	{"\\[ILLEGAL\\]\\s*\\{\\s*((\\s*|\\S*)*)\\}"};
	boost::regex files_expr		{"\\[FILE\\]:\\s*(\\S*)[\\s\\S]*"};
	boost::regex path_expr		{"\\/[\\S*]*"};

	if( !regex_search (fileContent, what, modtxt_expr) )
	{
		error_msg = "Couln't find the path to mod.txt in "+par->cfg+"\n"; 
		goto error;
	}
	par->modtxt = what[1];

	if( !regex_search (fileContent, what, input_expr) )
	{
		error_msg = "Couln't find the path to input.csv in "+par->cfg+"\n"; 
		goto error;
	}
	par->input = what[1];

	if( !regex_search (fileContent, what, files_expr) )
	{
		error_msg = "Couln't find the path to the directory for backup files in "+par->cfg+"\n"; 
		goto error;
	}

	par->files = what[1];
	
	if( regex_search (fileContent, what, illegal_expr) )	
	{
		string illegal = what[1];
		string::const_iterator begin = illegal.begin();
    	string::const_iterator end = illegal.end();
		for(int i=0; regex_search(begin, end, what, path_expr );i++ )
		{
			(*par->illegal)[i] = what[0];
			begin = what[0].second;
		}
	}
		
	if( regex_search (fileContent, what, warning_expr) )
	{
		string warning = what[1];
		string::const_iterator begin = warning.begin();
    	string::const_iterator end = warning.end();
		for(int i=0; regex_search(begin, end, what, path_expr );i++ )
		{
			(*par->warning)[i] = what[0];
			begin = what[0].second;
		}
	}

	if(par->files[(par->files).length()-1]!='/')
	{
		error_msg = "ERROR : \""+par->files+"\" \'/\' at the end is missing";
		goto error;
	}

	if(!exists_file(par->files))
	{
		error_msg = "Path : \""+par->files+"\" does not exist!";
		goto error;
	}

	if(!exists_file(par->input))
	{
		error_msg = "Inputfile : \""+par->input+"\" does not exist!";
		goto error;
	}

	
	file.close();


	return;

	error:
		cerr<<error_msg<<endl;
		file.close();
		exit(-1);
}

int main(int argc, char **argv)
{
	string cfg = "";
	string project_path = "";
	
	#ifdef PROJECTPATH
	string tmp = PROJECTPATH;
	project_path = tmp.substr(0,tmp.length()-4);
	#endif

	cfg = project_path+"cfg/paths.cfg";
    int opt = 0;
    int r = 0;
    int verbose = 0;
    //int c = 0;
    while ((opt = getopt(argc,argv,"rvc:d")) != EOF)
        switch(opt)
        {
            case 'r': r = 1; cout <<" restore is enabled"<<r <<endl; break;
            case 'v': verbose = 1; cout <<" verbose is enabled "<< verbose <<endl; break;
            //case 'c': c = 1; cfg = optarg; break;
            case '?': fprintf(stderr, "usuage is \n -a : for enabling a \n -b : for enabling b \n -c: <value> ");
            default: cout<<endl; abort();
        }

    if( !exists_file(cfg) )
    {

       	cerr<< "Error: Cfg file: \""<< cfg << "\" does not exist\n";
     	return -1;
    }
	


	Parameter param(verbose, r,cfg); 
	param.projectpath = project_path;
	cout << project_path << endl;
	
	fill_param(&param);
	if(verbose)
		print_param( param );

	for (unsigned int i = 0; i < param.warning->size (); i++)
    cout << (*param.warning)[i] << endl;
    for (unsigned int i = 0; i < param.illegal->size (); i++)
    cout << (*param.illegal)[i] << endl;
	

	cout << "ADRESSE: "<< &param.illegal << " - "<<&param.warning<<endl;
    Controller controller(param);
    
    if(r)
    {
     controller.restoreFiles();
     controller.cleanDirs(param.files,"*.v*");
     controller.cleanDirs(param.files,"*.txt");
     return 0;
    }

    controller.start();

    return 0;
}