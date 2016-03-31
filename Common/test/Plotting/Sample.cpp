#include <Sample.hpp>
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