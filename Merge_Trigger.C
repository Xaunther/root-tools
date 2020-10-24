//Script to merge trigger lines with an OR
#include <string>
#include <iostream>
#include "Functions/Filereading.h"
#include "Functions/ArrayTools.h"
#include "Functions/Filesaving.h"
using namespace std;

//Read, merge, save
void Merge_Trigger(string inputfile, string outputfile)
{
	int N = 0;
	string* varlist = ReadVariables(N, inputfile);
	string mergedlist = MergeArray_OR(varlist, N);
	SaveVariables(&mergedlist, 1, outputfile);
}


#if !defined(__CLING__)
int main(int argc, char** argv)
{
	switch (argc - 1)
	{
	case 2:
		Merge_Trigger(*(new string(argv[1])), *(new string(argv[2])));
		break;
	default:
		cout << "Wrong number of arguments (" << argc << ") for " << argv[0] << endl;
		return (1);
		break;
	}
	return 0;
}
#endif
