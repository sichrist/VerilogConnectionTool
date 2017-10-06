/*
* @Author: simon
* @Date:   2017-10-04 12:29:52
* @Last Modified by:   simon
* @Last Modified time: 2017-10-04 17:14:29
*/

# include "Cutter.h"

Cutter::Cutter( string str, char delim )
{
	set( str, delim);
}

Cutter::~Cutter()
{
	delete [] positions;
}

void Cutter::cnt( )
{
	size = 0;
	for( unsigned int i = 0; i < data.length(); i++)
		if( data[i] == delimiter )
			size++;		
}

void Cutter::cut( )
{
	size = 0;
	for( unsigned int i = 0; i < data.length(); i++)
		if( data[i] == delimiter )
		{
			positions[size] = i;
			size++;
		}
}

void Cutter::set( string str, char delim )
{
	data 		= str;
	delimiter 	= delim;
	size 		= 0;
	endbit 		= 0;
	dir 		= 0;
	stairway 	= 0;
	s_ 			= 0;
	e_ 			= data.length();
	p_l = p_r 	= NULL;

	cnt();

	positions = new size_t[size];
	for( unsigned int i=0; i< size; i++)
		positions[i] = 0;

	cut();
}

void Cutter::begin()
{
	p_l 	= &s_;
	p_r 	= &positions[0];
	dir 	= 1;
	endbit 	= 1;
	stairway= 0;
}

void Cutter::end()
{

	p_r 	= &e_;
	p_l 	= &positions[size-1];
	dir 	= -1;
	endbit 	= 1;
	stairway= 0;

}

inline void Cutter::posDir( )
{

	if( p_r == &positions[size-1] )
	{
		p_l = p_r;
		p_r = &e_;
		return;
	}
	else if( p_r == &e_ )
	{
		endbit = 0;
		p_l = p_r = NULL;
		return;
	}

	p_r++;
	p_l = p_r;
	p_l--;
}

inline void Cutter::negDir( )
{
	if( p_l == &positions[0] )
	{
		p_r = p_l;
		p_l = &s_;
		return;

	}
	else if( p_l == &s_ )
	{
		endbit = 0;
		p_l = p_r = NULL;
		return;
	}

	p_l--;
	p_r = p_l;
	p_r++;
}

string Cutter::nextstep()
{
	stairway = true;
	return next();
}

string Cutter::next()
{
	if( p_r == NULL || p_l == NULL || dir == 0 )
	{

		return "";
	}
	size_t start = 0;
	size_t len = 0;

	if( stairway )
	{
		start = s_;
	}
	else
	{
		start 	= *p_l;
	}
		len 	= (*p_r) - start;
		len = abs((int)len);

	if( dir > 0 )
		posDir();

	else
		negDir();


	if(data[start] == delimiter && !stairway)
	{
		++start;
		if(len == 0)
			return "";
		--len;
	}
	return data.substr(start,len);
}

bool Cutter::fin()
{
	return endbit;
}

string Cutter::getdata()
{
	return data;
}

char Cutter::getdelimiter()
{
	return delimiter;
}

size_t Cutter::getposleft()
{
	return *p_l;
}

size_t Cutter::getposright()
{
	return *p_r;
}

size_t Cutter::getsize()
{
	return size;
}

int Cutter::getdir()
{
	return dir;
}
