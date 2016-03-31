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
  double totWeight ;
  tree -> SetBranchAddress("totWeight", &totWeight);
  double weightLumi = (xsection*lumi)/Nevents;
  double totWeightWithLumi;
  TBranch * br = tree -> Branch("totEventWeight", &totWeightWithLumi, "totEventWeight/D"); 
  std::cout << FileName << std::endl;
  
  for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++)
  {
    tree -> GetEntry(iEntry); 
    totWeightWithLumi = totWeight*weightLumi;
    br -> Fill();
  }
  
  tree -> Write();

}

void addWeightSamples()
{
  double lumi = 2300.;
  std::string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_31March2016/";

  //electron channel
  addWeight(prefix + "ttbar_ele.root", 831.76, lumi, 196937036);
  addWeight(prefix + "WW_ele.root", 49.997, lumi, 1924400);
  addWeight(prefix + "WZ_ele.root", 10.71, lumi, 19742520);
  addWeight(prefix + "s-ch_ele.root", 10.32, lumi, 998400);
  addWeight(prefix + "t-ch_ele.root", 136.02, lumi, 19938230); 
  addWeight(prefix + "tW-ch-top_ele.root", 71.7, lumi, 1000000); 
  addWeight(prefix + "tW-ch-antitop_ele.root", 71.7, lumi, 999400); 
  addWeight(prefix + "WJets_Ht100To200_ele.root", 1345.0*1.21, lumi, 10205377); 
  addWeight(prefix + "WJets_Ht200To400_ele.root", 359.7*1.21, lumi, 4949568); 
  addWeight(prefix + "WJets_Ht400To600_ele.root", 48.91*1.21, lumi, 1943664); 
  addWeight(prefix + "WJets_Ht600To800_ele.root", 12.05*1.21, lumi, 3767766); 
  addWeight(prefix + "WJets_Ht800To1200_ele.root", 5.501*1.21, lumi, 1568277);
  addWeight(prefix + "WJets_Ht1200To2500_ele.root", 1.329*1.21, lumi, 246239); 
  addWeight(prefix + "WJets_Ht2500ToInf_ele.root", 0.03216*1.21, lumi, 251982); 

  //muon channel
  addWeight(prefix + "ttbar_mu.root", 831.76, lumi, 196937036);
  addWeight(prefix + "WW_mu.root", 49.997, lumi, 1924400);
  addWeight(prefix + "WZ_mu.root", 10.71, lumi, 19742520);
  addWeight(prefix + "s-ch_mu.root", 10.32, lumi, 998400);
  addWeight(prefix + "t-ch_mu.root", 136.02, lumi, 19938230); 
  addWeight(prefix + "tW-ch-top_mu.root", 71.7, lumi, 1000000); 
  addWeight(prefix + "tW-ch-antitop_mu.root", 71.7, lumi, 999400); 
  addWeight(prefix + "WJets_Ht100To200_mu.root", 1345.0*1.21, lumi, 10205377); 
  addWeight(prefix + "WJets_Ht200To400_mu.root", 359.7*1.21, lumi, 4949568); 
  addWeight(prefix + "WJets_Ht400To600_mu.root", 48.91*1.21, lumi, 1943664); 
  addWeight(prefix + "WJets_Ht600To800_mu.root", 12.05*1.21, lumi, 3767766); 
  addWeight(prefix + "WJets_Ht800To1200_mu.root", 5.501*1.21, lumi, 1568277);
  addWeight(prefix + "WJets_Ht1200To2500_mu.root", 1.329*1.21, lumi, 246239); 
  addWeight(prefix + "WJets_Ht2500ToInf_mu.root", 0.03216*1.21, lumi, 251982); 
  
 
}
