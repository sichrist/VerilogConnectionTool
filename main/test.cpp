/*
* @Author: simon
* @Date:   2017-08-11 16:05:33
* @Last Modified by:   simon
* @Last Modified time: 2017-10-04 17:08:56
*/

#include <iostream>

using namespace std;

# include "Cutter.h"
# include "Path.h"

int main ()
{

  
  string tmp = " ";
  Cutter cutter(".Das.ist.ein.Test.",'.');
  cout << "\n from start \n";
  cutter.begin();
  while( cutter.fin() )
  {
    cout <<"\""<< cutter.next()<<"\""<<endl;
  }
  
  cutter.end();
  cout << "\n from end \n";
  while( cutter.fin() )
  {
    cout <<"\""<< cutter.next()<<"\""<<endl;
  }
  cout << "---------"<<cutter.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;
  Cutter cutter1("Das.ist.ein.Test.",'.');
  cout << "\n from start \n";
  cutter1.begin();
  while( cutter1.fin() )
  {
    cout <<"\""<<cutter1.next()<<"\""<<endl;
  }
  cout << "\n from end \n";
  cutter1.end();
  while( cutter1.fin() )
  {
    cout << "\""<<cutter1.next()<<"\""<<endl;
  }
  cout << "---------"<<cutter1.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;  
  Cutter cutter2(".Das.ist.ein.Test",'.');
  cout << "\n from start \n";
  cutter2.begin();
  while( cutter2.fin() )
  {
    cout << "\""<<cutter2.next()<<"\""<<endl;
  }
  cout << "\n from end \n";
  cutter2.end();
  while( cutter2.fin() )
  {
    cout <<"\""<< cutter2.next()<<"\""<<endl;
  }
  cout << "---------"<<cutter2.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;  
  Cutter cutter3("Das.ist.ein.Test",'.');
  cout << "\n from start \n";
  cutter3.begin();
  while( cutter3.fin() )
  {
    cout <<"\""<< cutter3.next()<<"\""<<endl;
  }
  cout << "\n from end \n";
  cutter3.end();
  while( cutter3.fin() )
  {
    cout <<"\""<< cutter3.next()<<"\""<<endl;
  }
  cout << "---------"<<cutter3.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;

//############################################################

  cout << "########## STAIRWAYS TO HEAVEN ##############"<<endl;
  cout << "\n from start \n";
  cutter.begin();
  while( cutter.fin() )
  {
    cout <<"\""<< cutter.nextstep()<<"\""<<endl;
  }
  
  cutter.end();
  cout << "\n from end \n";
  while( cutter.fin() )
  {
    cout <<"\""<< cutter.nextstep()<<"\""<<endl;
  }
  cout << "---------"<<cutter.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;
  
  cout << "\n from start \n";
  cutter1.begin();
  while( cutter1.fin() )
  {
    cout <<"\""<<cutter1.nextstep()<<"\""<<endl;
  }
  cout << "\n from end \n";
  cutter1.end();
  while( cutter1.fin() )
  {
    cout << "\""<<cutter1.nextstep()<<"\""<<endl;
  }
  cout << "---------"<<cutter1.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;  
  cout << "\n from start \n";
  cutter2.begin();
  while( cutter2.fin() )
  {
    cout << "\""<<cutter2.nextstep()<<"\""<<endl;
  }
  cout << "\n from end \n";
  cutter2.end();
  while( cutter2.fin() )
  {
    cout <<"\""<< cutter2.nextstep()<<"\""<<endl;
  }
  cout << "---------"<<cutter2.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;  
  cout << "\n from start \n";
  cutter3.begin();
  while( cutter3.fin() )
  {
    cout <<"\""<< cutter3.nextstep()<<"\""<<endl;
  }
  cout << "\n from end \n";
  cutter3.end();
  while( cutter3.fin() )
  {
    cout <<"\""<< cutter3.nextstep()<<"\""<<endl;
  }
  cout << "---------"<<cutter3.getdata()<<"-----------"<<endl;
  cout << "-------------------------------------------"<<endl;

  cout << "########## PATH TEST ##############"<<endl;

  Path path0("/home/simon/Arbeitsverzeichnis/VerilogConnectionTool/main");
  cout<<"1.) " << path0.getfilename() <<endl;
  cout<<"2.) " << path0.getfolder() <<endl;
  cout<<"3.) " << path0.getfullpath() <<endl;
  cout<<"4.) " << path0.dirExist() <<endl;
  cout<<"5.) " << path0.fileExist() <<endl;

  return 0;

}
