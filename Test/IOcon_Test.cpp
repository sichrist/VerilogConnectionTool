# include "IOcon.h"
# include <iostream>
# include <string>
using namespace std;

int main()
{

	IOcon iocon("    input     [0:8] test  |output out(1)  |");

	cout << iocon.buildNormal(MODU)<<endl;
	cout << iocon.buildNormal(OPEN_PORT)<<endl;
	cout << iocon.buildNormal(MODU_REV)<<endl;
	return 0;
}