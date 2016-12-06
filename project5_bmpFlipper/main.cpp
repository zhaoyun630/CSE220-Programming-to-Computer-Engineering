#include <iostream>
#include "bmp_parse.h"

using namespace std;

int main(int argc, char* argv[])
{
	bmpParse parser;
	int ret = 0;
        
	if(argc == 1)
	{
		cout << "Usage: " << argv[0] << " <input file> <operation> <output file>" << endl;
		return 0;
	}				      

	if(argc == 2)
	{
		cout << "What would you like me to do to " << argv[1] <<"?" << endl;
		return 0;
	}
	if(argc == 3)
	{
		cout << "Gimme an output file, buddy." << endl;
		return 0;
	}

	ret = parser.readfile(argv[1]);
	if(ret == 0)
	{
		cout << "Read Success!" << endl;
		if(!strcmp(argv[2],"flipx"))
		{
			cout << "flipping along x ..." << endl;
			parser.flipx();
		}		
		else if(!strcmp(argv[2],"flipy"))
		{
			cout << "flipping along y ..." << endl;
			parser.flipy();
		}
		else if(!strcmp(argv[2],"rot90"))
		{
			cout << "rotating 90 degrees ..." << endl;
			parser.rot90();
		}
		else
		{
			cout << "Can't '" << argv[2] <<"' on " << argv[3] << ". Don't know how :(" << endl;
		}

		if(parser.writefile(argv[3]) != 0)
			cout << "Failed to " << argv[2] << ". Cannot open '" << argv[3] <<"' for writing." << endl;
		else
			cout << "Completed." << endl;
	}
	else
	{
		cout << "Cannot open: '" << argv[1] << "' for reading!" << endl;
	}

	return 0;
}
