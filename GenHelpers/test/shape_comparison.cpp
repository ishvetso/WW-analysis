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

void shape_comparison(){

  gStyle->SetCanvasColor(kWhite);
//gStyle->SetTitleFillColor(kWhite);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
//gPad->SetGrid();
	
  TFile file_signal("/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/GenHelpers/test/M_WW_gen.root");
  TFile file_SM("/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/Common/test/crab_projects/crab_WW_gen_SM/results/M_WW_gen-SM.root");

  TTree * tree_signal = (TTree * )file_signal.Get("makeWWmass/Tree");
  TTree * tree_SM = (TTree * )file_SM.Get("makeWWmass/Tree");

  double M_WW;
  std::map<std::string, double> *aTGCWeights = 0;

  tree_SM -> SetBranchAddress("m_WW", &M_WW);

  tree_signal -> SetBranchAddress("m_WW", &M_WW);
  tree_signal -> SetBranchAddress("aTGCWeights", &aTGCWeights);
  

  TH1F *hist_SM = new TH1F("SM", "SM", 30, 400., 5000.);
  std::vector <TH1F *> hists_signal; 
  for (unsigned int iHist = 0; iHist < 8; iHist ++) {
    TH1F *hist = new TH1F(("signal" + to_string(iHist + 1)).c_str(), ("signal" + to_string(iHist + 1)).c_str(), 30, 400., 10000.);
    hist -> SetLineColor(kBlue+iHist*2);
    hists_signal.push_back(hist);
  }
  



  /*for (unsigned int iEntry = 0; iEntry < tree_SM -> GetEntries(); iEntry ++){
  	tree_SM -> GetEntry(iEntry);
  	hist_SM -> Fill(M_WW, weight_SM);

  }*/

  std::map<std::string, double>::iterator it;
  for (unsigned int iEntry = 0; iEntry < tree_signal -> GetEntries(); iEntry ++){
  	tree_signal -> GetEntry(iEntry);
    int iWeight = 0;
  	 for (it = aTGCWeights -> begin(); it != aTGCWeights -> end(); it++){
      //std::cout << iWeight << "  " << it -> second << std::endl;
      hists_signal.at(iWeight) -> Fill(M_WW, it -> second);
      std::cout << it -> first << "  " << it -> second << std::endl;
      iWeight++;

     }
     aTGCWeights -> clear();

  }

  setTDRStyle();
  TCanvas *c1= new TCanvas("c1","canvas",1200,800);
  c1-> cd();
  c1 -> SetLogy();
  TLegend *leg = new TLegend(0.2,0.3,0.5,0.7);
  leg -> SetFillColor(kWhite);  

  for (auto iHist: hists_signal){
    iHist -> DrawNormalized("HISTSAME");
  }

  /*hist_SM -> SetLineColor(kBlue);
  hist_signal -> SetLineColor(kRed);
  hist_SM -> Draw();
  hist_signal -> Draw("SAME");*/

  CMS_lumi( c1, 4, 0 );
  c1 -> SaveAs( "SMvsSignal.png");

}