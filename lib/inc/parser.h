/*
* @Author: simon
* @Date:   2017-08-11 16:05:15
* @Last Modified by:   simon
* @Last Modified time: 2017-08-23 14:50:10
*/
# ifndef PARSER_H
# define PARSER_H

# include <iostream>
# include <string>
# include <map>
using namespace std;


class Parser
{
private:
  string str;
  char delimiter;
    map < int, string > parsed;
    map < int, string > sparsed;
  int size;
  int pos;
public:
    Parser ();
   ~Parser ();

  void parse ();
  void singleparse ();
  char getdelimiter ();
  void setstart ();
  void setend ();
  void setstring (string str);
  void setdelimiter (char delim);
  void checkforbracket ();

  string getsamehier (Parser * pars);
  string getnext ();
  string getnextbw ();
  string getstring ();
  string getb4 ();
  string getb4bw ();

  int getsize ();
};

# endif
