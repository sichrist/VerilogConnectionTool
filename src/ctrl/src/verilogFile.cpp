/*
* @Author: simon
* @Date:   2017-09-11 16:32:26
* @Last Modified by:   simon
* @Last Modified time: 2017-09-28 15:07:03
*/

# include "Filehandler.h"
# include "Path.h"

verilogFile::verilogFile ()
{

  start = 0;
  end = 0;
  path = "";
  exec = NULL;
}

verilogFile::verilogFile (string path)
{

  start = 0;
  end = 0;
  exec = NULL;
  this->path = path;
}

verilogFile::verilogFile (string bin, string path)
{
  
  exec = new Exec (bin, path);
  start = 0;
  end = 0;
  this->path = path;
}

verilogFile::~verilogFile ()
{

  if (exec != NULL)
    {
      //getit ("\nEOF\n");
      delete exec;
      exec = NULL;
    }
}

/***

	parse a verilogfile by searching for definitions and instantiations

***/

void
verilogFile::parse ()
{
  exec->start ();
}

void
verilogFile::cout_ (long start, long end)
{
  if (start == end || start > end)
    return;
  char c = 0;

  int ctr = 0;

  read.open (path.c_str ());
  read.seekg (start);

  while (read.tellg () < end)
    {
      read.get (c);

      if (read.tellg () < start + 200 || read.tellg () > end - 200)
	cout << c;
      if (ctr == 200)
	cout << "\n .............\n\n .............\n";
      ctr++;
      if (read.eof ())
	break;
    }

  read.close ();

}

string verilogFile::getit (string str)
{
  string
    ret = "";
  ret = exec->get (str);

  size_t
    found = ret.find ("NOT FOUND");

  if (!found)
    return ret;

  found = ret.find ("-");

  if (!found)
    return ret;

  string
    start,
    end;

  start = ret.substr (1, found);
  end = ret.substr (found + 1, ret.length ());

  long
    st = atol (start.c_str ());

  long
    en = atol (end.c_str ());

  this->start = st;
  this->end = en;
  return ret;
}

static void
split (string str, char delim, string * i1, string * i2)
{
  std::size_t found = str.find (delim);

  string
  one (str, 0, found);

  string
  two (str, found + 1, str.length ());

  *i1 = one;
  *i2 = two;
}

void
verilogFile::insert (long start, long end, string buffer)
{

	Path pathObj(path,'/');
	string pth = "";

	pathObj.end();

	pth = "tmp_" + pathObj.next ();

	if (end == 0)
	{
		cerr << "Module in " << path << " not found" << endl;
		return;
	}

	this->start = 0;
	this->end = end;
	write.open (pth.c_str (), fstream::trunc);
	write.close ();
	cpy (pth);
	write.open (pth.c_str (), fstream::app);
	write << buffer;
	write.close ();

	this->start = end;
	this->end = -1;
	cpy (pth);

  //string cmd = "diff -uNr " + path + " " + pth + " >> history.diff";
  //system ((const char *) cmd.c_str ());

	string tmp_path = this->path;
	this->path = pth;
	pth = tmp_path;
	fullcpy (pth);

	tmp_path = this->path;
	this->path = pth;
	pth = tmp_path;

  //i++;
}

static void
getnbrs (string str, long *start1, long *end1, long *start2, long *end2)
{
  static int j = 0;

  long i1 = 0, i2 = 0;

  long start = 0, end = 0, start_ = 0, end_ = 0;

  string eins, zwei, uno, duo;

  for (unsigned int i = 0; i < str.length (); i++)
    {
      if (str[i] == '(')
	i1 = i;
      if (str[i] == ')')
	i2 = i;
    }

  if (i1 == 0 && i2 == 0)
    return;

  string tmp (str, i1 + 1, i2 - i1 - 1);

  split (tmp, ',', &eins, &zwei);
  split (eins, '-', &uno, &duo);

  start = atol (uno.c_str ());
  end = atol (duo.c_str ());

  split (zwei, '-', &uno, &duo);
  start_ = atol (uno.c_str ());
  end_ = atol (duo.c_str ());

  j++;

  *start1 = start;
  *start2 = start_;
  *end1 = end;
  *end2 = end_;
}

static void
parseret (string str, long *se)
{
  unsigned int cut1, cut2, cut3, i;

  for (i = 0; i < str.length (); i++)
    if (str[i] == '\n')
      break;

  cut1 = i;
  i++;
  for (; i < str.length (); i++)
    if (str[i] == '\n')
      break;
  cut2 = i;

  for (; i < str.length (); i++)
    if (str[i] == '\n')
      break;
  cut3 = i;

  string mods (str, 0, cut1);

  string inst (str, cut1, cut2 - cut1);

  string gene (str, cut2, cut3 - cut2);

  string wire (str, cut2, str.length () - cut3);

  getnbrs (mods, &se[0], &se[1], &se[2], &se[3]);
  getnbrs (inst, &se[4], &se[5], &se[6], &se[7]);
  getnbrs (gene, &se[8], &se[9], &se[10], &se[11]);
  getnbrs (wire, &se[12], &se[13], &se[14], &se[15]);


}

bool
checkcomment (string str)
{

  if (str.find ("YES") != std::string::npos)
    {
      return true;
    }
  return false;
}

bool
checkstyle (string str)
{
  if (str.find ("OLD") != std::string::npos)
    {
      return true;
    }
  return false;
}

bool
checkgenerate (string str)
{
  if (str.find ("GEN") != std::string::npos)
    {
      return true;
    }
  return false;
}

bool verilogFile::getcomment ()
{
  return this->comment;
}

bool verilogFile::getstyle ()
{
  return this->style;
}

void
verilogFile::checkVerilogFile (string modname)
{

  string modu_def;

  for (int i = 0; i < 12; i++)
    se[i] = 0;

  modu_def = getit ("module|" + modname);

  parseret (modu_def, se);

  string t1, t2;

  split (modu_def, '\n', &t1, &t2);
  comment = checkcomment (t1);
  style = checkstyle (t1);
  split (t2, '\n', &t1, &t2);
  wire_comment = checkcomment (t2);

}


void
verilogFile::checkVerilogFile (string modname, string instname,
			       string defmodname)
{

  string inst_def;

  for (int i = 0; i < 16; i++)
    se[i] = 0;

  inst_def = getit (modname + "|" + instname + "|" + defmodname);
  parseret (inst_def, se);

  string t1, t2;

  split (inst_def, '\n', &t1, &t2);
  split (t2, '\n', &t1, &t2);
  comment = checkcomment (t1);
  wire_comment = checkcomment (t2);
  generate = false;

}

void
verilogFile::change (IOcon con, Type type)
{
  string comment_begin = "";

  string comment_end = "";

  string context = "";
  switch (type)
    {
    case MODU_REV:
    case MODU:
      if (!comment)
	{
	  comment_begin = con.getModComment ();
	  comment_end = con.getEndComment ();
	}

      if (this->style == true)
	{
	  context = comment_begin + con.buildOld (0, type) + comment_end;
	  insert (0, se[0], context);
	  se[2] += context.length ();
	  context = comment_begin + con.buildOld (1, type) + comment_end;
	  insert (0, se[2], context);
	}
      else
	{
	  context = comment_begin + con.buildNormal (type) + comment_end;
	  insert (0, se[0], context);
	}
      break;


    case INST:
      if (!comment)
	{
	  comment_begin = con.getInsComment ();
	  comment_end = con.getEndComment ();
	}
      context = comment_begin + con.buildNormal (type) + comment_end;
      insert (0, se[4], context);
      break;

    case WIRE:
      if (!wire_comment)
	{
	  comment_begin = con.getWirComment ();
	  comment_end = con.getEndComment ();
	}
      context = con.buildWire ();
      insert (0, se[14], comment_begin + context + comment_end);
      break;
    case OPEN_PORT:

      if (!comment)
	{
	  comment_begin = con.getInsComment ();
	  comment_end = con.getEndComment ();
	}

      context = comment_begin + con.buildNormal (type) + comment_end;
      insert (0, se[4], context);
      break;

    default:
      break;
    }
}


void
verilogFile::loadnewfile (string path)
{
  this->path = path;
  this->comment = false;
  this->style = false;
  exec->send ("[LOAD:" + path + ";");
}
