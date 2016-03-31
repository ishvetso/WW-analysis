#ifndef  VAR
#define VAR

#include <iostream>
#include <string>
#include <sstream>
#include <TBranch.h>
#include <TTree.h>

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

  Double_t value_d;
  Int_t    value_i;
  TBranch* branch;

  EDataType type;

  Double_t value();
	
	void SetRange(double xlow, double xhigh);
  void Initialize(TTree* tree);
};

#endif