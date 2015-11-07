#ifndef  VAR
#define VAR

#include <iostream>
#include <string>
#include <sstream>

/*
 * Structure to work with variable
 */

using namespace std;

struct range 
{
  double low;
  double high;
  
};

struct Var 
{
	string VarName;
  string Title;
	range Range;
	
	void SetRange(double xlow, double xhigh);
};

void Var::SetRange(double xlow, double xhigh)
{

  range RangeX;
  RangeX.low = xlow;
  RangeX.high = xhigh;
  Range = RangeX;
}
#endif