#ifndef systhelper
#define systhelper

#include <map>
#include <vector>
#include <TTree.h>
#include <TH1D.h>
#include <var.hpp>

class SystHelper{
private:
  
  std::map <std::string, std::vector<std::string>> VariablesAffected;
  std::vector<std::string> VarsJEC;
  std::vector<std::string> VarsLeptonEn;
  std::vector<std::string> VarsLeptonRes;
  std::vector<std::string> VarsUnclEn;
  std::vector<std::string> VarsJER;
  std::vector<std::string> VarsJetRes;
  std::string NominalSelection;
 
  // histograms for varname, syst
  std::map< std::pair<std::string,std::string>, TH1D* >  hist_SystUp;
  std::map< std::pair<std::string,std::string>, TH1D* >  hist_SystDown;

  // additional histograms
  std::vector<std::map<std::string, TH1D * > >histsAdd_Up;
  std::vector<std::map<std::string, TH1D * > > histsAdd_Down;
  
  //selection strings
  std::map<std::string, std::string> selection_Up, selection_Down;
  std::string selstring;

  // for selection from tree
  std::map<std::string, TTreeFormula*> selectionUpInFormula, selectionDownInFormula;
  TTreeFormula *nominalSelection;
  //weights for systematics
  std::vector<std::string> WeightNameSystematics;
  std::map<std::string, Var*> weightsUp;
  std::map<std::string, Var*> weightsDown;
  
public:
  std::vector<std::string> ListOfSystematics;
  SystHelper();
  SystHelper(std::string selection);
  void initTree(TTree* tree);
  void AddVar(Var* var, TH1D* refhist);
  void AddSyst(std::map<std::string, TH1D * > & Up, std::map<std::string, TH1D * > & Down);
  void eval(Var* var, TH1D * hist_nominal);
  void fill(vector <Var>* var, std::map<std::pair<std::string, std::string>, Var*> & SystematicsVarMapUp_, std::map<std::pair<std::string, std::string>, Var*> & SystematicsVarMapDown_, double weight);
  bool isAffectedBySystematic(Var  var, std::string systematic);
};


#endif