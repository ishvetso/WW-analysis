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
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
	
  TFile file_signal("/afs/cern.ch/work/i/ishvetso/public/samples_74X_15January2016/WW-aTGC-mu.root");
  TTree * tree_signal = (TTree * )file_signal.Get("treeDumper/BasicTree");

  double M_WW;
  std::map<std::string, double> *aTGCWeights = 0;

  tree_signal -> SetBranchAddress("m_lvj", &M_WW);
  tree_signal -> SetBranchAddress("aTGCWeights", &aTGCWeights);

  std::vector <std::string> titles;
  titles.push_back("CWWWL2 = 12., CWL2 = 0., CBL2 = 0.");
  titles.push_back("CWWWL2 = -12., CWL2 = 0., CBL2 = 0.");
  titles.push_back("CWWWL2 = 0., CWL2 = 20., CBL2 = 0.");
  titles.push_back("CWWWL2 = 0., CWL2 = -20., CBL2 = 0.");
  titles.push_back("CWWWL2 = 0., CWL2 = 0., CBL2 = 60.");
  titles.push_back("CWWWL2 = 0., CWL2 = 0., CBL2 = -60.");
  titles.push_back("CWWWL2 = -12., CWL2 =-20., CBL2 = -60.");
  titles.push_back("CWWWL2 = 0., CWL2 = 0., CBL2 = 0.");

  std::vector<Color_t> colors;
  colors.push_back(kRed);
  colors.push_back(kGreen);
  colors.push_back(kMagenta);
  colors.push_back(kCyan);
  colors.push_back(kOrange);
  colors.push_back(kBlue);
  colors.push_back(kViolet);
  colors.push_back(kBlack);

  std::string Title = "RECO level";
  

 std::vector <TH1F *> hists_signal; 
  for (unsigned int iHist = 0; iHist < 8; iHist ++) {
    TH1F *hist = new TH1F(("signal" + to_string(iHist + 1)).c_str(), ("signal" + to_string(iHist + 1)).c_str(), 30, 400., 10000.);
    hist -> Sumw2();
    hist -> SetLineColor(colors[iHist]);
    hist -> SetLineWidth(2.);
    hist -> GetXaxis() -> SetTitle("M_{WW}");
    hist -> GetYaxis() -> SetTitle("Nevents");
    hists_signal.push_back(hist);
  }


  std::map<std::string, double>::iterator it;
  double lumi = 2093.917403402;
  for (unsigned int iEntry = 0; iEntry < tree_signal -> GetEntries(); iEntry ++){
  	tree_signal -> GetEntry(iEntry);
    int iWeight = 0;
  	 for (it = aTGCWeights -> begin(); it != aTGCWeights -> end(); it++){
        hists_signal.at(iWeight) -> Fill(M_WW, (it -> second)*lumi/20.);
        iWeight++;
     }
     aTGCWeights -> clear();

  }

  setTDRStyle();
  TCanvas *c1= new TCanvas("c1","canvas",1200,800);
  c1-> cd();
  c1 -> SetLogy();
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
  leg -> SetFillColor(kWhite); 
  leg -> SetHeader(Title.c_str()); 

  int iTitle =0;
  for (auto iHist: hists_signal){
    iHist -> Draw("HISTSAMEE1");
    leg->AddEntry(iHist, titles[iTitle].c_str(),"l");
    iTitle ++;
  }

  leg -> Draw("SAME");
  CMS_lumi( c1, 4, 0 );
  c1 -> SaveAs( (Title +".png").c_str());

}