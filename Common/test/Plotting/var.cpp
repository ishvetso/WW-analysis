#include <var.hpp>
#include <assert.h>
#include <iostream>

void Var::SetRange(double xlow, double xhigh)
{

  range RangeX;
  RangeX.low = xlow;
  RangeX.high = xhigh;
  Range = RangeX;
}

  void Var::Initialize(TTree* tree){
  branch = tree->GetBranch(VarName.c_str());

  TClass* tc = new TClass();
  branch->GetExpectedType(tc,type);
  delete tc;

  std::cout<< "connecting branch "<< VarName << " with type " << type << std::endl;

  if (type==kDouble_t)
    tree->SetBranchAddress(VarName.c_str(),&value_d,&branch);
  if (type==kInt_t)
    tree->SetBranchAddress(VarName.c_str(),&value_i,&branch);

  assert("tried to initialize unsupported datatype");

}

Double_t Var::value(){
  if(type==kInt_t){
    return (Double_t) value_i;
  }
  return value_d;

