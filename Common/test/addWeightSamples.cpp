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
  addWeight("/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/test/crab_projects/crab_WW_analysis_ele/results/WW_ele.root", 118.7, 5.590, 989608); //WW
  addWeight("/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/test/crab_projects/crab_WJets_analysis_ele/results/WJets_ele.root", 20508.9, 5.590, 24089991); //W + jets
  addWeight("/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/test/crab_projects/crab_TTBar_analysis_ele/results/ttbar_ele.root",  831.76, 5.590, 4994250); //ttbar
 
}
