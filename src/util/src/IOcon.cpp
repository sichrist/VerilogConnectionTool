
#include "IOcon.h"


const string  IOcon::MODULE_COMMENT =
  "\n\n//-=-=-=-=-=-=-=-=-=-=-\n//\n//The following IO's\n//are automatically \n//generated \n//\n";
const  string  IOcon::INSTAN_COMMENT =
  "\n\n//-=-=-=-=-=-=-=-=-=-=-\n//\n//The following connections\n//are automatically \n//generated \n//\n";
const  string  IOcon::WIRE_COMMENT =
  "\n\n//-=-=-=-=-=-=-=-=-=-=-\n//\n//The following wires\n//are automatically \n//generated \n//\n";
const  string  IOcon::END_COMMENT = "\n\n//\n//-=-=-=-=-=-=-=-=-=-=-\n\n";


static  string invert (string type)
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
            context += "\t\t" + cons[i].name + "\t,\n";
    }
    else
    {
        for (int i = 0; i < size; i++)
            if (type == MODU_REV)
            {
                context += "\t\t" + invert (cons[i].type)+ " \t" + cons[i].bitwith + " \t" + cons[i].name + "\t;\n";
            }
            else
            {
                context += "\t\t" + cons[i].type + " \t" + cons[i].bitwith + " \t" + cons[i].name + "\t;\n";
            }
    }

    
  return context;
}


string IOcon::buildNormal (Type type)
{
  string context = "";

    if (type == INST)
    {
        for (int i = 0; i < size; i++)
        {
            context += "\t\t." + cons[i].name + "\t( " + cons[i].name + " ),\n";
        }
    return context;
    }
    
    if (type == OPEN_PORT)
    {
        for (int i = 0; i < size; i++)
        {
            context += "\t\t." + cons[i].name + "\t( "+openPortInp+" ),\n";
        }
      return context;
    }
    
    for (int i = 0; i < size; i++)
    {
        if (type == MODU_REV)
    	{
    	  context += "\t\t" + invert (cons[i].type) + " \t";
    	}
        else
        {
    	  context += "\t\t" + cons[i].type + " \t";
        }
      context += cons[i].bitwith + "\t";
      context += cons[i].name + "\t,\n";
    }
    return context;
}


string IOcon::buildWire ()
{

    string context = "";
    for (int i = 0; i < size; i++)
    {
        context += "wire\t" + cons[i].bitwith + "\t" + cons[i].name + "\t;\n";
    }
    return context;
}


IOcon::IOcon (string ios)
{
 
  status = 0;
  cons = NULL;
  error_msg = "";
  openPortInp = " ";

  this->ios = ios;

  parse ();

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


// Removes leading spaces and spaces a the end

string IOcon::removespaceat( string ios )
{
	string tmp;
	unsigned int i = 0;

	for( i = 0; i < ios.size(); i++)
	{
		if( ios[i] != ' ')
			break;
	}

	tmp = ios.substr(i, ios.size() - i);
	ios = tmp;

	for( i = ios.size()-1; i > 0; i-- )
	{
		if( ios[i] != ' ')
			break;
	}
	tmp = ios.substr(0,i+1);
	ios = tmp;

	return ios;
}


inline Cons IOcon::split( string io)
{
    Cons ret;

    io = removespaceat(io);

    Cutter cut(io,' ');
    cut.decimate();
    cut.end();

  
    ret.name = cut.next();
    ret.bitwith = cut.next();
    ret.type = cut.next();

    if(ret.type == "")
    {
        ret.type = ret.bitwith;
        ret.bitwith = "";
    }

	if( cut.getsize() > 2 )
	{
		status = 1;
		error_msg = "Couldn't parse ios correctly: \""+io+"\"\n";
	}

	return ret;
}


IOcon::~IOcon ()
{
	delete [] cons;
}


void IOcon::parse()
{
  string tmp;
  stack<string> data;

  // seperate Io's
  Cutter cutter(ios,'|');
  cutter.begin();
  while( cutter.fin() )
  {
    tmp=cutter.next();
    if(tmp != "")
      data.push(tmp);
  }
  size = data.size();
  cons = new Cons[size];

  for(int i=0; i < size; i++)
  {
    cons[i] = split( data.top() );
    data.pop();
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
