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
  string _Unit;
	range Range;
  bool logscale;
  bool HasUnits_;

  Double_t value_d;
  Int_t    value_i;
  TBranch* branch;

  EDataType type;

  Double_t value();
	
	void SetRange(double xlow, double xhigh);
  void Initialize(TTree* tree);
  bool HasUnits();
  string Unit();
};

#endif