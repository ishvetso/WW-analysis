#ifndef systhelper
#define systhelper

#include <map>
#include <vector>
#include <TTree.h>
#include <TH1D.h>
#include <var.hpp>

class SystHelper{
private:
  std::vector<std::string> ListOfSystematics;
  std::map <std::string, std::vector<std::string>> VariablesAffected;
  std::vector<std::string> VarsJEC;
  std::vector<std::string> VarsLeptonEn;
  std::vector<std::string> VarsLeptonRes;
  std::vector<std::string> VarsUnclEn;
 
  // histograms for varname, syst
  std::map< std::pair<std::string,std::string>, TH1D* >  hist_SystUp;
  std::map< std::pair<std::string,std::string>, TH1D* >  hist_SystDown;
  
  //selection strings
  std::map<std::string, std::string> selection_Up, selection_Down;
  std::string selstring;

  // for selection from tree
  std::map<std::string, TTreeFormula*> selectionUpInFormula, selectionDownInFormula;
  
public:
  SystHelper(std::string selection);
  void initTree(TTree* tree);
  void AddVar(Var* var, TH1D* refhist);
  void eval(Var* var, TH1D * hist_nominal);
  void fill(Var* var, double weight);
};


#endif