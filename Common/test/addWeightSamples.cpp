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

void addWeight(string FileName, float xsection, float lumi, int Nevents)
{
  TFile file(FileName.c_str(), "UPDATE");
  TDirectoryFile *tdir = (TDirectoryFile*) file.Get("treeDumper");
  TTree * tree = (TTree*) tdir -> Get("Basic Info");
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
  addWeight("/afs/cern.ch/work/i/ishvetso/samples_PHYS14/Wjets.root", 20508.9,20000., 10017462); //ttbar
  addWeight("/afs/cern.ch/work/i/ishvetso/samples_PHYS14/ttbar.root", 831.76 ,20000., 2991609 );//W + jets
}
