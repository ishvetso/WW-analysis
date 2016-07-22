#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TDirectoryFile.h>
#include <iostream>
#include <iostream>


/*
 * Small macro to add weights corresponding to the cross-sections
 * 
 * Compile by: 
 * root -l addWeightSamples.cpp+
 */

int Nevents(std::string filename){
   TFile file(filename.c_str(), "READ");
  TTree * tree = (TTree*) file.Get("GenWeights/Tree");
  double gentWeight;
  tree -> SetBranchAddress("genWeight", &gentWeight);
  int sum = 0;
  for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++)
  {
    tree -> GetEntry(iEntry); 
    sum += gentWeight/std::abs(gentWeight);
  }
  return sum;
}
void addWeight(string FileName, float xsection, float lumi)
{
  int Nevents_ = Nevents(FileName);
  TFile file(FileName.c_str(), "UPDATE");
  TTree * tree = (TTree*) file.Get("treeDumper/BasicTree");
  double totWeight,totWeight_BTagUp,totWeight_BTagDown, totWeight_MistagUp, totWeight_MistagDown, totWeight_LeptonIDUp, totWeight_LeptonIDDown ;
  tree -> SetBranchAddress("totWeight", &totWeight);
  tree -> SetBranchAddress("totWeight_BTagUp", &totWeight_BTagUp);
  tree -> SetBranchAddress("totWeight_BTagDown", &totWeight_BTagDown);
  tree -> SetBranchAddress("totWeight_MistagUp", &totWeight_MistagUp);
  tree -> SetBranchAddress("totWeight_MistagDown", &totWeight_MistagDown);
  tree -> SetBranchAddress("totWeight_LeptonIDUp", &totWeight_LeptonIDUp);
  tree -> SetBranchAddress("totWeight_LeptonIDDown", &totWeight_LeptonIDDown);
  double weightLumi = (xsection*lumi)/Nevents_;
  double totWeightWithLumi, totWeightWithLumi_MistagUp, totWeightWithLumi_MistagDown, totWeightWithLumi_BTagUp, totWeightWithLumi_BTagDown,  totWeightWithLumi_LeptonIDUp, totWeightWithLumi_LeptonIDDown;
  TBranch * br = tree -> Branch("totEventWeight", &totWeightWithLumi, "totEventWeight/D"); 
  TBranch * br_MistagUp = tree -> Branch("totEventWeight_MistagUp", &totWeightWithLumi_MistagUp, "totEventWeight_MistagUp/D"); 
  TBranch * br_MistagDown = tree -> Branch("totEventWeight_MistagDown", &totWeightWithLumi_MistagDown, "totEventWeight_MistagDown/D"); 
  TBranch * br_BTagUp = tree -> Branch("totEventWeight_BTagUp", &totWeightWithLumi_BTagUp, "totEventWeight_BTagUp/D"); 
  TBranch * br_BTagDown = tree -> Branch("totEventWeight_BTagDown", &totWeightWithLumi_BTagDown, "totEventWeight_BTagDown/D");
  TBranch * br_LeptonIDUp = tree -> Branch("totEventWeight_LeptonIDUp", &totWeightWithLumi_LeptonIDUp, "totEventWeight_LeptonIDUp/D"); 
  TBranch * br_LeptonIDDown = tree -> Branch("totEventWeight_LeptonIDDown", &totWeightWithLumi_LeptonIDDown, "totEventWeight_LeptonIDDown/D");
  std::cout << FileName << std::endl;
  std::cout << "Number of events (effective):" << Nevents_ << std::endl;
  
  for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++)
  {
    tree -> GetEntry(iEntry); 
    totWeightWithLumi = totWeight*weightLumi;
    totWeightWithLumi_BTagUp = totWeight_BTagUp*weightLumi;
    totWeightWithLumi_BTagDown = totWeight_BTagDown*weightLumi;
    totWeightWithLumi_MistagUp= totWeight_MistagUp*weightLumi;
    totWeightWithLumi_MistagDown = totWeight_MistagDown*weightLumi;
    totWeightWithLumi_LeptonIDUp= totWeight_LeptonIDUp*weightLumi;
    totWeightWithLumi_LeptonIDDown = totWeight_LeptonIDDown*weightLumi;
    br -> Fill();
    br_MistagUp -> Fill();
    br_MistagDown -> Fill();
    br_BTagUp -> Fill();
    br_BTagDown -> Fill();
    br_LeptonIDUp -> Fill();
    br_LeptonIDDown -> Fill();
  }
  
  tree -> Write();

}

void addWeightSamples()
{
  double lumi = 2300.;
  std::string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_12July2016/";

  //electron channel
  addWeight(prefix + "WW-tot_ele.root", 49.997, lumi);
  addWeight(prefix + "WZ_ele.root", 11.46, lumi);
  addWeight(prefix + "s-ch_ele.root", 10.32*0.33, lumi);
  addWeight(prefix + "t-ch-tot_ele.root", 216.99*0.33, lumi); 
  addWeight(prefix + "tW-ch-top_ele.root", 35.6, lumi); 
  addWeight(prefix + "tW-ch-antitop_ele.root", 35.6, lumi); 
  addWeight(prefix + "WJets_HT-100To200-tot_ele.root", 1345.0*1.21, lumi); 
  addWeight(prefix + "WJets_HT-200To400-tot_ele.root", 359.7*1.21, lumi); 
  addWeight(prefix + "WJets_HT-400To600_ele.root", 48.91*1.21, lumi); 
  addWeight(prefix + "WJets_HT-600To800_ele.root", 12.05*1.21, lumi); 
  addWeight(prefix + "WJets_HT-800To1200-tot_ele.root", 5.501*1.21, lumi);
  addWeight(prefix + "WJets_HT-1200To2500-tot_ele.root", 1.329*1.21, lumi); 
  addWeight(prefix + "WJets_HT-2500ToInf-tot_ele.root", 0.03216*1.21, lumi); 
  addWeight(prefix + "ttbar-powheg-tot_ele.root", 831.76, lumi); 
  //muon channel

  addWeight(prefix + "WW-tot_mu.root", 49.997, lumi);
  addWeight(prefix + "WZ_mu.root", 11.46, lumi);
  addWeight(prefix + "s-ch_mu.root", 10.32*0.33, lumi);
  addWeight(prefix + "t-ch-tot_mu.root", 216.99*0.33, lumi); 
  addWeight(prefix + "tW-ch-top_mu.root", 35.6, lumi); 
  addWeight(prefix + "tW-ch-antitop_mu.root", 35.6, lumi); 
  addWeight(prefix + "WJets_HT-100To200-tot_mu.root", 1345.0*1.21, lumi); 
  addWeight(prefix + "WJets_HT-200To400-tot_mu.root", 359.7*1.21, lumi); 
  addWeight(prefix + "WJets_HT-400To600_mu.root", 48.91*1.21, lumi); 
  addWeight(prefix + "WJets_HT-600To800_mu.root", 12.05*1.21, lumi); 
  addWeight(prefix + "WJets_HT-800To1200-tot_mu.root", 5.501*1.21, lumi);
  addWeight(prefix + "WJets_HT-1200To2500-tot_mu.root", 1.329*1.21, lumi); 
  addWeight(prefix + "WJets_HT-2500ToInf-tot_mu.root", 0.03216*1.21, lumi); 
  addWeight(prefix + "ttbar-powheg-tot_mu.root", 831.76, lumi); 
  
 
}