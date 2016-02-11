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

void aTGC_var( std::string var, std::string filename, std::string Title="RECO level", std::string xaxislabel=""){

  gStyle->SetCanvasColor(kWhite);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
	
  TFile file_signal(filename.c_str());
  TTree * tree_signal = (TTree * )file_signal.Get("treeDumper/BasicTree");

  double M_WW;
  double pt;
  std::vector<double> *aTGCWeights = 0;

  tree_signal -> SetBranchAddress(var.c_str(), &M_WW);

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

 std::vector <TH1F *> hists_signal; 
  for (unsigned int iHist = 0; iHist < 8; iHist ++) {
    TH1F *hist = new TH1F(("signal" + to_string(iHist + 1)).c_str(), ("signal" + to_string(iHist + 1)).c_str(), 30, 400., 10000.);
    hist -> Sumw2();
    hist -> SetLineColor(colors[iHist]);
    hist -> SetLineWidth(2.);
    hist -> GetXaxis() -> SetLabelSize(0.03);
    hist -> GetXaxis() -> SetTitle(xaxislabel.c_str());
    hist -> GetYaxis() -> SetTitle("Nevents");
    hists_signal.push_back(hist);
    tree_signal -> Project(("signal" + to_string(iHist + 1)).c_str(),var.c_str(), ("aTGCWeights[" + to_string(iHist) + "]*2093.917403402/20.").c_str());
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
  c1 -> SaveAs( (var + "_" + Title +".png").c_str());

  delete c1;
}


void shape_comparison(){
  aTGC_var("m_lvj", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/Common/test/crab_projects/crab_aTGC_mu_5February2016/results/WW-aTGC-mu.root", "RECO", "m_{WW}");
  aTGC_var("jet_pt", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/Common/test/crab_projects/crab_aTGC_mu_5February2016/results/WW-aTGC-mu.root", "RECO", "jet p_{T}");

  aTGC_var("m_WW", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/GenHelpers/test/crab_projects/crab_WW_gen_signal_update/results/M_WW_gen.root", "GEN", "m_{WW}");
  aTGC_var("pt_Wplus", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/GenHelpers/test/crab_projects/crab_WW_gen_signal_update/results/M_WW_gen.root", "GEN", "p_{T,W}");

}