#ifndef  SAMPLE
#define SAMPLE

#include <iostream>
#include <string>
#include <sstream>
#include <TColor.h>

/*
 * Structure to work with Sample
 */

using namespace std;

struct Sample 
{
	string selection;
	Color_t color;
	vector <string> filenames;
	string Processname; // name of the process
	
	Sample();
	void SetParameters(string Processname_, string selection_, Color_t color_);
	void SetFileNames(string filename_);
	void ReSet();	
};
#endif