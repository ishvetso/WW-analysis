#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <iostream>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm> 
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/tdrstyle.C>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/CMS_lumi.cpp>

void calculateKFactor(){
	
  TFile file_signal("/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/Common/test/crab_projects/crab_WW_gen_signal/results/M_WW_gen-signal.root");
  TFile file_SM("/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/Common/test/crab_projects/crab_WW_gen_SM/results/M_WW_gen-SM.root");

  TTree * tree_signal = (TTree * )file_signal.Get("makeWWmass/Tree");
  TTree * tree_SM = (TTree * )file_SM.Get("makeWWmass/Tree");

  double M_WW;
  double weight8;
  double refXsec;

  tree_SM -> SetBranchAddress("m_WW", &M_WW);

  tree_signal -> SetBranchAddress("m_WW", &M_WW);
  tree_signal -> SetBranchAddress("weight8", &weight8);
  tree_signal -> SetBranchAddress("refXsec", &refXsec);

  TH1F *hist_SM = new TH1F("SM", "SM", 30, 400., 2500.);
  TH1F *hist_signal = new TH1F("signal", "signal", 30, 400., 2500.);

  float sigma_SM = 49.997;
  float lumi = 2090.0;
  float N_events_SM = 1951600.;
  float N_events_signal = 1998600.;

  float weight_SM = sigma_SM/N_events_SM;

  for (unsigned int iEntry = 0; iEntry < tree_SM -> GetEntries(); iEntry ++){
  	tree_SM -> GetEntry(iEntry);
  	hist_SM -> Fill(M_WW);

  }

  hist_SM -> Scale(sigma_SM/hist_SM->Integral());

  for (unsigned int iEntry = 0; iEntry < tree_signal -> GetEntries(); iEntry ++){
  	tree_signal -> GetEntry(iEntry);
  	hist_signal -> Fill(M_WW, weight8);

  }

   TCanvas *c1= new TCanvas("c1","canvas",1200,800);

  hist_SM -> SetLineColor(kBlue);
  hist_signal -> SetLineColor(kRed);
  hist_SM -> Draw();
  hist_signal -> Draw("SAME");

  c1 -> SaveAs( "SMvsSignal.png");

}