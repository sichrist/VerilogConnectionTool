
#include "Filehandler.h"


const string
  IOcon::MODULE_COMMENT =
  "\n\n//-=-=-=-=-=-=-=-=-=-=-\n//\n//The following IO's\n//are automatically \n//generated \n//\n";
const
  string
  IOcon::INSTAN_COMMENT =
  "\n\n//-=-=-=-=-=-=-=-=-=-=-\n//\n//The following connections\n//are automatically \n//generated \n//\n";
const
  string
  IOcon::WIRE_COMMENT =
  "\n\n//-=-=-=-=-=-=-=-=-=-=-\n//\n//The following wires\n//are automatically \n//generated \n//\n";
const
  string
  IOcon::END_COMMENT = "\n\n//\n//-=-=-=-=-=-=-=-=-=-=-\n\n";

static
  string
invert (string type)
{
  if (!type.find ("input"))
    {
      return "output";
    }
  if (!type.find ("output"))
    return "input";
  return type;
}



/***

	builds a string depend on section

	module name(

		section = 0
	);
		section = 1

***/

string IOcon::buildOld (bool section, Type type)
{
  string context = "";


  if (!section)
    {
      for (int i = 0; i < size; i++)
	context += "\t\t" + *cons[i].name + "\t,\n";
    }
  else
    {
      for (int i = 0; i < size; i++)
	if (type == MODU_REV)
	  {
	    context += "\t\t" + invert (*cons[i].type)
	      + " \t" + *cons[i].bitwith + " \t" + *cons[i].name + "\t;\n";
	  }
	else
	  {
	    context +=
	      "\t\t" + *cons[i].type + " \t" + *cons[i].bitwith + " \t" +
	      *cons[i].name + "\t;\n";
	  }
    }

    
  return context;
}

string
IOcon::buildNormal (Type type)
{
  string context = "";

  if (type == INST)
    {
      for (int i = 0; i < size; i++)
	{
	  context += "\t\t." + *cons[i].name + "\t( " + *cons[i].name + " ),\n";
	}
      return context;
    }
  if (type == OPEN_PORT)
    {
      for (int i = 0; i < size; i++)
	{
	  context += "\t\t." + *cons[i].name + "\t( "+openPortInp+" ),\n";
	}
      return context;
    }
  for (int i = 0; i < size; i++)
    {
      if (type == MODU_REV)
	{
	  context += "\t\t" + invert (*cons[i].type) + " \t";
	}
      else
	{
	  context += "\t\t" + *cons[i].type + " \t";
	}
      context += *cons[i].bitwith + "\t";
      context += *cons[i].name + "\t,\n";
    }
  return context;
}

string
IOcon::buildWire ()
{

  string context = "";
  for (int i = 0; i < size; i++)
    {
      context += "wire\t" + *cons[i].bitwith + "\t" + *cons[i].name + "\t;\n";
    }
  return context;
}


IOcon::IOcon (Parser ios)
{
 
  size = 0;
  status = 0;
  cons = NULL;
  error_msg = "";
  openPortInp = " ";
  this->ios = ios;

  parse ();

 
  if( status )
    return;
  int ctr = size;

  for (int i = 0; i < ctr; i++)
    {
      this->name += *cons[i].name + ",\n";
    }

}

inline int cntChar( string in, char del )
{
    int nmbr = 0;
    for( unsigned int i = 0; i<in.size(); i++ )
    {
        if( in[i] == del )
            nmbr++;
    }
    return nmbr;
}
// splits the string "type bitwith name" and places it in cons

inline bool IOcon::splitIOinput( string ios )
{
    if(ios == "")
        return 1;
    int nbrspace = 0;
    string name_tmp = "";
    string bitw_tmp = "";
    string type_tmp = "";
    std::size_t found;   
    Parser spaceparser;

    nbrspace = cntChar( ios, ' ' );
    if( nbrspace > 2 || nbrspace < 1 )
    {
        error_msg = "Input parsing went wrong: \""+ios+"\"\n"+"max 2 spaces min 1!\n"+"Example: type bitwith name|type name\n";
        goto error;
    }

    
    spaceparser.setstring( ios );
    spaceparser.setdelimiter( ' ' );
    spaceparser.singleparse();
   
    name_tmp = spaceparser.getnextbw();
    bitw_tmp = spaceparser.getnextbw();
    found = bitw_tmp.find("[");
    if(!found)
    {
      type_tmp = spaceparser.getnextbw();
    }
    else
    {
      type_tmp = bitw_tmp;
      bitw_tmp = "";
    }

    // process the input of open ports

    
    found = name_tmp.find("(");

    if( found!=std::string::npos )
    {
        std::size_t found_tmp = found;
        found = name_tmp.find(")");
        if( found!=std::string::npos )
        {
            openPortInp = name_tmp.substr(found_tmp+1, found - found_tmp -1 );
        }
        string tmp = name_tmp.substr(0,found_tmp);
        name_tmp = tmp;

    }

    if( name_tmp == ""|| type_tmp == ""){   
        error_msg = "Input parsing went wrong: \""+type_tmp+" "+bitw_tmp+" "+name_tmp+"\"\n";
        goto error;
    }

    cons[size].name = new string(name_tmp);
    cons[size].bitwith = new string(bitw_tmp);
    cons[size].type = new string(type_tmp);

    size++;

    return 0;

    error:
        status = 1;
        return 1;
}

IOcon::~IOcon ()
{
  for (int i = 0; i < size; i++)
    {

/*
      delete cons[i].name;
      delete cons[i].type;
      delete cons[i].bitwith;
*/
    }

  //delete[]cons;
}


void IOcon::parse ()
{

  string tmp = "";
  string tmp1 = "";
  string tmp2 = "";
  string input = "";

  ios.singleparse ();
  tmp = " ";
  std::size_t found;

  if (cons != NULL)
    delete[]cons;

  cons = new Cons[ios.getsize ()];

  while (tmp != "")
    {
      tmp = ios.getnextbw ();
      found = tmp.find("#");

      if( found!=std::string::npos )
      {
        error_msg = "Character \'#\' found in io-part of the inputfile.\n Remove this Character!\n";
        status = 1;
        return;
      }

      tmp1 = tmp1 + "#" + tmp;
    }

  tmp2 = ios.getstring ();
  ios.setstring (tmp1);
  ios.setdelimiter ('#');
  ios.singleparse ();

  tmp = ios.getnextbw ();
  tmp = " ";

  while (tmp != "")
    {
        input = ios.getnextbw ();
        if( splitIOinput( input ) )
            break;

    }

    
}

string IOcon::geterrormsg()
{
    return error_msg;
}

bool IOcon::getstatus()
{
    return status;
}

int IOcon::getsize ()
{
  return this->size;
}

Cons *IOcon::getcons ()
{
  return this->cons;
}

Parser IOcon::getios ()
{
  return this->ios;
}

string
IOcon::getDef ()
{
  return this->def;
}

string
IOcon::getDefrev ()
{
  return this->defrev;
}

string
IOcon::getIns ()
{
  return this->ins;
}

string
IOcon::getname ()
{
  return this->name;
}

string
IOcon::getModComment ()
{
  return this->MODULE_COMMENT;
}

string
IOcon::getInsComment ()
{
  return this->INSTAN_COMMENT;
}

string
IOcon::getWirComment ()
{
  return this->WIRE_COMMENT;
}

string
IOcon::getEndComment ()
{
  return this->END_COMMENT;
}
