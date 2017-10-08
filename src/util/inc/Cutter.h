# ifndef CUTTER_H
# define CUTTER_H
# include <iostream>
# include <string>

using namespace std;

// Saves the positions of a delimiter in a string an returns certain kind of pieces

class Cutter
{

protected:
	
	// String to cut
	string data;

	// positions of delimiter
	size_t *positions;

	// length of pos
	size_t size;

	// delimiter
	char delimiter;

	// pointer to last
	size_t *p_l;

	//pointer to next
	size_t *p_r;

	// start of data -> 0
	size_t s_;

	// end of data -> data.length()
	size_t e_;

	//direction
	int dir;

	// p_r or p_l (depends on begin(), end() ) position won't change
	bool stairway;

	// signalize the end 
	bool endbit;

	// cuts the String
	void cut( );

	// Counts the amount of a character in str
	void cnt( );

	// manipulates p_r & p_l starting from the beginning of data
	void posDir(  );

	// manipulates p_r & p_l starting from the end of data
	void negDir(  );




public:
	
	// Construktor
	Cutter( string str, char delim );

	// Destructor
	~Cutter();

	// Recycle Cutter
	void set( string str, char delim );

	// start from the beginning of a data
	void begin();

	// start from the end of a data
	void end();

	// returns the next string between delimiters
	string next();

	// returns the string from start/end and the delimiter p_l/p_r points to
	string nextstep();

	// returns True if there is something to read, else returns 0
	bool fin();

	// get-methods
	string getdata();

	// makes sure that a delimiter do not follow a delimiter
	void decimate();

	char getdelimiter();

	size_t getposleft();
	size_t getposright();
	size_t getsize();

	int getdir();
};

# endif