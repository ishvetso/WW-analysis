#ifndef  SAMPLE
#define SAMPLE

#include <iostream>
#include <string>
#include <sstream>

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

Sample::Sample()
{}

void Sample::SetParameters( string Processname_, string selection_, Color_t color_)
{
	selection = selection_;
	color = color_;
	Processname = Processname_;
}

void Sample::SetFileNames(string filename_)
{
	filenames.push_back(filename_);

}

void Sample::ReSet()
{
    filenames.clear();
}
#endif