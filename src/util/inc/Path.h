# ifndef PATH_H
# define PATH_H

# include "Cutter.h"
# include <dirent.h>

bool exists_dir (const std::string & name);
bool exists_file (const std::string & name);

class Path : public Cutter
{

protected:

	// Name of file
	string filename;

	// Path to folder which contains the file
	string folderpath;


public:
	// Constructor for a "normal" Path wir delim '/'
	Path(string path );

	Path();
	
	// Constructor for special paths
	Path(string path, char delim);

	~Path();

	// Recycle the object
	void set( string path, char delim );
	
	void set( string path );

	// Returns the filename
	string getfilename();

	// Returns the folder which contains the file
	string getfolder();

	// Returns the Path we set this object to
	string getfullpath();

	// Returns true if the directory this object was initialized with exists
	bool dirExist();

	// Returns true if the file this object was initialized with exists
	bool fileExist();

	// creates the file if it doesn't exists
	bool createFile();

	// creates the folder, if it doesn't exists
	bool createFolder();

};
# endif
