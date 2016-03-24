#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TTreeFormula.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TLegend.h>
#include <TEfficiency.h>
#include <TColor.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>
#include <stdio.h>      
#include <stdlib.h>  
#include <TASImage.h>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/tdrstyle.C>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/CMS_lumi.cpp>
#include <boost/algorithm/string/replace.hpp>
#include "boost/algorithm/string.hpp"
#include "aTGCsAnalysis/Common/test/Plotting/Sample.hpp"

TH2D * convertEffToTH2F(TEfficiency * eff_){
	TH2D *hist_eff = (TH2D*) eff_->GetPassedHistogram();
	for (unsigned int iBinX = 1; iBinX <= hist_eff -> GetXaxis() -> GetNbins(); iBinX ++){
		for (unsigned int iBinY = 1; iBinY <= hist_eff -> GetYaxis() -> GetNbins(); iBinY ++){
			hist_eff -> SetBinContent(iBinX, iBinY, eff_  -> GetEfficiency(eff_->GetGlobalBin(iBinX,iBinY)));
			hist_eff -> SetBinError(iBinX, iBinY, std::max(eff_  -> GetEfficiencyErrorUp(eff_->GetGlobalBin(iBinX,iBinY)),eff_  -> GetEfficiencyErrorLow(eff_->GetGlobalBin(iBinX,iBinY)) ));
		}

	}
	return hist_eff;
}

void dumpPlotCategory(std::string filename, std::string ProcessName, std::string category, std::string outDir){

	if (category != "udsg" && category != "c" && category != "b") {
		std::cerr << "Wrong category used .." << std::endl;
		exit(0);
	}
	gStyle->SetOptStat(0);
	//gStyle->SetOptTitle(0);
	TFile eff_file(filename.c_str());
	TEfficiency *eff = (TEfficiency*)eff_file.Get(("BtagAnalyzer/h2_BTaggingEff_" + category ).c_str());
	setTDRStyle();
	gStyle->SetPalette(55);
	gStyle->SetNumberContours(255);
	if (category != "udsg")gStyle->SetPaintTextFormat("4.4f");
	else gStyle->SetPaintTextFormat("4.5f");
	gStyle->SetPadLeftMargin(0.15);
	gStyle->SetPadRightMargin(0.15);
	TCanvas *c1=  new TCanvas("c1","canvas",1200,800);
	c1 -> SetLogx();

	TH2D * hist_eff  = convertEffToTH2F(eff);

	hist_eff -> Draw("COLZTEXTE");
	CMS_lumi( c1, 4, 0 );
	c1 -> SetTitle(ProcessName.c_str());
	c1-> SaveAs((outDir + "/" + ProcessName + "_" + category + ".png").c_str());
	delete c1;
}

void dumpEffPlot(){
	std::string filename ="eff_ttbar_mu.root";
	std::vector<std::string> categories;
	categories.push_back("b");
	categories.push_back("c");
	categories.push_back("udsg");
	std::string OutPrefix_  = "plots";
	system(("mkdir -p " + OutPrefix_ ).c_str());
	Sample s;
	vector<Sample> samples;
	
	string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_6_3_patch1/src/aTGCsAnalysis/TreeMaker/data/";
	
	s.SetParameters("ttbar_ele", "", kRed);
 	s.SetFileNames( prefix + "eff_ttbar_ele.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("ttbar_mu", "", kRed);
 	s.SetFileNames( prefix + "eff_ttbar_mu.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WW_ele", "", kRed);
 	s.SetFileNames( prefix + "eff_WW_ele.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WW_mu", "", kRed);
 	s.SetFileNames( prefix + "eff_WW_mu.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WW-aTGC_ele", "", kRed);
 	s.SetFileNames( prefix + "eff_WW-aTGC_ele.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WW-aTGC_mu", "", kRed);
 	s.SetFileNames( prefix + "eff_WW-aTGC_mu.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WZ_ele", "", kRed);
 	s.SetFileNames( prefix + "eff_WZ_ele.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WZ_mu", "", kRed);
 	s.SetFileNames( prefix + "eff_WZ_mu.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WZ-aTGC_ele", "", kRed);
 	s.SetFileNames( prefix + "eff_WZ-aTGC_ele.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WZ-aTGC_mu", "", kRed);
 	s.SetFileNames( prefix + "eff_WZ-aTGC_mu.root");
	samples.push_back(s);
	s.ReSet();

	for (unsigned int iSample = 0; iSample < samples.size(); iSample ++ ){
		for (unsigned int iFileName = 0; iFileName < samples.at(iSample).filenames.size() ; iFileName++){
			for (auto category: categories){
				dumpPlotCategory(samples.at(iSample).filenames.at(iFileName), samples.at(iSample).Processname, category, OutPrefix_);
			}
		}
	}



}