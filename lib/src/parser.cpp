/*
* @Author: simon
* @Date:   2017-08-08 09:21:17
* @Last Modified by:   simon
* @Last Modified time: 2017-09-07 12:03:39
*/
#include "parser.h"

Parser::Parser ()
{
  size = 0;
  pos = 0;
}

Parser::~Parser ()
{

}

void
Parser::parse ()
{

  string tmp = str;

  int ctr = 0;

  int len = str.length ();

  pos = 0;
  parsed.clear ();
  sparsed.clear ();
  parsed[ctr++] = tmp;

  for (int i = len; i >= 0; i--)
    {
      if (tmp[i] == this->delimiter)
	{
	  parsed[ctr] = tmp.substr (0, i);
	  ctr++;
	}
    }
  size = ctr;
}

void
Parser::checkforbracket ()
{
  int l;

  string str;

  bool sp = false, p = false;

  for (int i = 0; i < size; i++)
    {
      str = parsed[i];
      l = str.length () - 1;
      if (str[l] == ']')
	{
	  p = true;
	  parsed[i] = "";
	}
      str = sparsed[i];
      l = str.length () - 1;
      if (str[l] == ']')
	{
	  sp = true;
	  sparsed[i - 1] = sparsed[i] + "." + sparsed[i - 1];
	}
      if (p)
	{
	  if (i == size)
	    {
	      size--;
	      break;
	    }
	  parsed[i] = parsed[i + 1];
	}

      if (sp)
	{
	  if (i == size)
	    {
	      size--;
	      break;
	    }
	  sparsed[i] = sparsed[i + 1];
	}
    }
}

string Parser::getsamehier (Parser * pars)
{
  string
    tmp;

  pars->setend ();

  if (parsed.size () == 0 && sparsed.size () == 0)
    {
      cerr << "NULL\n";
      return "";
    }

  if (parsed.size () != 0)
    {
      pars->setstart ();
      while (true)
	{
	  tmp = pars->getnext ();
	  if (tmp == "")
	    break;
	  for (int i = 0; i < size; i++)
	    {
	      if (tmp.compare (parsed[i]) == 0)
		{
		  pars->setstart ();
		  return tmp;
		}
	    }

	}
    }
  else
    {
      pars->setstart ();
      for (int j = 0; j < pars->getsize (); j++)
	{
	  tmp = pars->getnextbw ();
	  if (tmp == "")
	    break;
	  for (int i = size - 1; i >= 0; i--)
	    {
	      if (tmp.compare (sparsed[i]) == 0)
		{
		  pars->setstart ();
		  return tmp;
		}
	    }

	}
    }
  pars->setstart ();
  return "";
}

void
Parser::singleparse ()
{
  string tmp = str;

  int ctr = 0;

  parsed.clear ();
  sparsed.clear ();
  int len = str.length ();

  int len1 = str.length ();

  pos = 0;


  for (int i = len; i >= 0; i--)
    {

      if (tmp[i] == this->delimiter)
	{
	  sparsed[ctr] = tmp.substr (i + 1, len1 - i - 1);
	  len1 = i;
	  ctr++;
	}
    }

  sparsed[ctr] = tmp.substr (0, len1);
  ctr++;
  size = ctr;
}



string Parser::getstring ()
{
  return this->str;
}

char
Parser::getdelimiter ()
{
  return this->delimiter;
}

string Parser::getb4 ()
{
  pos--;
  if (pos < 0)
    pos = 0;

  string
    tmp = parsed[pos];

  return tmp.c_str ();
}

string Parser::getnext ()
{

  if (pos == size)
    return "";
  string
    tmp = parsed[pos];

  pos++;
  return tmp.c_str ();
}

string Parser::getnextbw ()
{
  if (pos > size)
    return "";
  string
    tmp = sparsed[pos];

  pos++;
  return tmp.c_str ();
}

string Parser::getb4bw ()
{
  pos--;
  if (pos < 0)
    pos = 0;

  string
    tmp = sparsed[pos];

  return tmp.c_str ();
}

int
Parser::getsize ()
{
  return this->size;
}

void
Parser::setend ()
{
  pos = size;
}

void
Parser::setstart ()
{
  pos = 0;
}

void
Parser::setstring (string str)
{
  this->str = str;
}

void
Parser::setdelimiter (char delim)
{
  this->delimiter = delim;
}
