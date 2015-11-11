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

void addWeight(string FileName, float xsection, float lumi, int Nevents)
{
  TFile file(FileName.c_str(), "UPDATE");
  TTree * tree = (TTree*) file.Get("treeDumper/BasicTree");
  float weight = (xsection*lumi)/Nevents;
  TBranch * br = tree -> Branch("weight", &weight, "weight/F"); 
  
  for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++)
  {
    br -> Fill();
  }
  
  tree -> Write();

}

void addWeightSamples()
{
  double lumi = 1547.184412658;
  std::string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_5November2015/";

  //electron channel
  addWeight(prefix + "ttbar_ele.root", 831.76, lumi, 96834559);
  addWeight(prefix + "WW_ele.root", 49.997, lumi, 1951600);
  addWeight(prefix + "WZ_ele.root", 10.71, lumi, 24714550);
  addWeight(prefix + "s-ch_ele.root", 10.32, lumi, 984400);
  addWeight(prefix + "t-ch_top_ele.root", 136.02, lumi, 3299800); 
  addWeight(prefix + "t-ch_antitop_ele.root", 80.95, lumi, 1680200); 
  addWeight(prefix + "tW-ch_ele.root", 71.7, lumi, 500000); 
  addWeight(prefix + "WJets_Ht100To200_ele.root", 1345.*1.21, lumi, 10152718); 
  addWeight(prefix + "WJets_Ht200To400_ele.root", 359.7*1.21, lumi, 5221599); 
  addWeight(prefix + "WJets_Ht400To600_ele.root", 48.91*1.21, lumi, 1745914); 
  addWeight(prefix + "WJets_Ht600ToInf_ele.root", 18.77*1.21, lumi, 1039152); 

  //muon channel
  addWeight(prefix + "ttbar_mu.root", 831.76, lumi, 96834559);
  addWeight(prefix + "WW_mu.root", 49.997, lumi, 1951600);
  addWeight(prefix + "WZ_mu.root", 10.71, lumi, 24714550);
  addWeight(prefix + "s-ch_mu.root", 10.32, lumi, 984400);
  addWeight(prefix + "t-ch_top_mu.root", 136.02, lumi, 3299800); 
  addWeight(prefix + "t-ch_antitop_mu.root", 80.95, lumi, 1680200); 
  addWeight(prefix + "tW-ch_mu.root", 71.7, lumi, 500000); 
  addWeight(prefix + "WJets_Ht100To200_mu.root", 1345.*1.21, lumi, 10152718); 
  addWeight(prefix + "WJets_Ht200To400_mu.root", 359.7*1.21, lumi, 5221599); 
  addWeight(prefix + "WJets_Ht400To600_mu.root", 48.91*1.21, lumi, 1745914); 
  addWeight(prefix + "WJets_Ht600ToInf_mu.root", 18.77*1.21, lumi, 1039152); 
  
 
}
