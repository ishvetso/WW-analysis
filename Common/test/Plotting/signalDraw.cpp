#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTreeFormula.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TPaveText.h"
#include <TStyle.h>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <algorithm> 
#include <Sample.cpp> 
#include <CMS_lumi.cpp>
#include <tdrstyle.C>

void signalDraw(){
    
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    setTDRStyle();

    std::string dir = "/afs/cern.ch/work/i/ishvetso/public/samples_76X_22July2016/";
	std::string channel = "mu";
	TFile file_WW((dir + "WW-aTGC_" + channel + ".root").c_str(), "READ");
	TFile file_WZ((dir + "WZ-aTGC_" + channel + ".root").c_str(), "READ");

	std::string selection = "(jet_pt > 200. && jet_tau2tau1 < 0.6  && Mjpruned < 105. && Mjpruned > 65. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900. && nbtag == 0";
	if (channel == "ele") selection += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") selection += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}

    std::string aTGC = "cwww";
    std::string aTGCLatexLabel;
    int NaTGC;
    if (aTGC == "cwww") {
        NaTGC = 1;
        aTGCLatexLabel = "c_{www}";
    }
    else if (aTGC == "cw") {
        NaTGC = 3;
        aTGCLatexLabel = "c_{w}";
    }
    else if (aTGC == "cb") {
        NaTGC = 5;
        aTGCLatexLabel = "c_{b}";
    }
    else {
        std::cerr << "Invalid atgc" << std::endl;
        exit(0);
    }
	std::vector<double> * aTGCWeights = 0;
	double totWeight;
	double Mjpruned;

	TTree * tree_WW = (TTree*)file_WW.Get("treeDumper/BasicTree");
	TTree * tree_WZ = (TTree*)file_WZ.Get("treeDumper/BasicTree");
	
	tree_WW -> SetBranchAddress("totWeight", &totWeight);//for signal sample use weight without lumi weight
    tree_WW -> SetBranchAddress("aTGCWeights", &aTGCWeights);
    tree_WW -> SetBranchAddress("Mjpruned", &Mjpruned);

    tree_WZ -> SetBranchAddress("totWeight", &totWeight);//for signal sample use weight without lumi weight
    tree_WZ -> SetBranchAddress("aTGCWeights", &aTGCWeights);
    tree_WZ -> SetBranchAddress("Mjpruned", &Mjpruned);

    TTreeFormula *signalSelection_WW = new TTreeFormula("signalSelection",selection.c_str(),tree_WW);//that should be without any weights!
    TTreeFormula *signalSelection_WZ = new TTreeFormula("signalSelection",selection.c_str(),tree_WZ);//that should be without any weights!

    TH1D *hist_WW = new TH1D("WW", "WW", 30, 65., 105.);
    TH1D *hist_WZ = new TH1D("WZ", "WZ", 30, 65., 105.);
    hist_WW -> Sumw2();
    hist_WZ -> Sumw2();

    for (unsigned int iEtry = 0; iEtry < tree_WW->GetEntries(); iEtry++)
    {
    	tree_WW -> GetEntry(iEtry);
    	if (signalSelection_WW -> EvalInstance()) hist_WW -> Fill(Mjpruned,totWeight*(aTGCWeights->at(NaTGC))*2300./20.);
        aTGCWeights->clear();
    }

    for (unsigned int iEtry = 0; iEtry < tree_WZ->GetEntries(); iEtry++)
    {
    	tree_WZ -> GetEntry(iEtry);
    	if (signalSelection_WZ -> EvalInstance()) hist_WZ -> Fill(Mjpruned,totWeight*(aTGCWeights->at(NaTGC))*2300./20.);
        aTGCWeights->clear();
    }

    TCanvas *c1=  new TCanvas("c1","canvas",1200,800);
    THStack *hs = new THStack("hs","Number of events");  
    
    hist_WW -> SetFillColor(kRed);
    hist_WZ -> SetFillColor(kBlue);

    TPaveText pt1(0.2,0.3,0.5,0.4,"NDC");
    pt1.SetTextFont(42);
    pt1.SetTextSize(0.05);
    pt1.SetFillColor(0);
    pt1.SetFillStyle(0);
    pt1.SetBorderSize(0);

    pt1.AddText("WW");
    pt1.AddText("category");


    TPaveText pt2(0.6,0.3,1.,0.4,"NDC");
    pt2.SetTextFont(42);
    pt2.SetTextSize(0.05);
    pt2.SetFillColor(0);
    pt2.SetFillStyle(0);
    pt2.SetBorderSize(0);

    pt2.AddText("WZ");
    pt2.AddText("category");


    TLegend *leg = new TLegend(0.7,0.7,0.95,0.9);
    leg->SetTextFont(42);
    leg->SetTextSize(0.05);
    if (channel == "mu")leg->SetHeader((aTGCLatexLabel + ", #mu#nu").c_str());
    else if (channel == "ele")leg->SetHeader((aTGCLatexLabel + ", e#nu").c_str());
    else {
        std::cerr << "Invalid channel" << std::endl;
        exit(0);
    }
    leg -> SetFillColor(kWhite);
    leg -> AddEntry(hist_WW, "WW", "f");
    leg -> AddEntry(hist_WZ, "WZ", "f");

    hist_WW->GetYaxis()->SetTitle("Number of events");
    hist_WW->GetXaxis()->SetTitle("M_{pruned}");
    hist_WW->GetYaxis()->SetRangeUser(0., 1.1*((hist_WW -> GetMaximum())  + (hist_WZ -> GetMaximum())) );
    hs -> Add(hist_WZ, "bar");
    hs -> Add(hist_WW, "bar");    
    
    hist_WW -> Draw();
    hs ->Draw("SAME");
    leg -> Draw("SAME");
    pt1.Draw("SAME");
    pt2.Draw("SAME");
    
    CMS_lumi( c1, 4, 0 );
    c1 -> Update();
    TLine *line = new TLine(85.,0.,85.,c1->GetUymax());
    line -> SetLineStyle(9);
    line -> Draw("SAME");
    c1 -> SaveAs(("signal-"+aTGC + "-"+ channel  +".pdf").c_str());
    

}