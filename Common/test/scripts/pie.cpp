#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TH1.h>
#include <TChain.h>
#include <TH2.h>
#include <THStack.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>
#include <TLegend.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>
#include <stdio.h>      
#include <stdlib.h>  
#include <TPie.h>
#include <aTGCsAnalysis/Common/test/Plotting/Sample.hpp>



double NEventsExpected(Sample s, std::string selection){

	
	TChain chain("BasicTree");
	for (unsigned int iFile = 0; iFile < s.filenames.size(); iFile ++){
		chain.Add(s.filenames.at(iFile).c_str());
	}
	TH1F * hist = new TH1F(("hist_" + s.Processname).c_str(), ("hist_" + s.Processname).c_str(), 1, 0., 10000.);
	chain.Project(("hist_" + s.Processname).c_str(), "nPV", selection.c_str());
	double N = (double) hist -> Integral();

	return N;

	delete hist;
}

void pie()
{
	
	Sample s;
	std::vector<Sample> samples;
	std::string channel = "ele";
	
	std::string prefix = "/afs/cern.ch/work/i/ishvetso/public/samples_74X_15January2016/";
	string defaulCuts = "(jet_pt > 200. && jet_tau2tau1 < 0.5  && jet_mass_pruned < 130. && jet_mass_pruned > 40. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2.";
	if (channel == "ele") defaulCuts += " && l_pt > 140. && pfMET > 80. )"; 
	else if (channel == "mu") defaulCuts += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}
	string addOnCutWjets = defaulCuts +  " * ( (jet_mass_pruned < 65. || jet_mass_pruned > 105. ) && nbtag == 0) ";
	string addOnCutTtbar = defaulCuts +  " * (nbtag > 0 )";
	
	
	string MCSelection = "weight*PUweight*(genWeight/abs(genWeight))*( " + addOnCutWjets + " )";

	
	s.SetParameters("WW", MCSelection, kRed);
 	s.SetFileNames( prefix + "WW_"+ channel + ".root");
 	s.SetFileNames( prefix + "WZ_"+ channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("W+jets", MCSelection, kGreen);
 	s.SetFileNames(prefix + "WJets_Ht100To200_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht200To400_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht400To600_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht600ToInf_" + channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("ttbar", MCSelection, kOrange);
 	s.SetFileNames(prefix + "ttbar_" + channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("#splitline{Single}{Top}", MCSelection, kBlue);
 	s.SetFileNames(prefix + "t-ch_top_" + channel + ".root");
 	s.SetFileNames(prefix + "t-ch_antitop_" + channel + ".root");
 	s.SetFileNames(prefix + "tW-ch_" + channel + ".root");
 	s.SetFileNames(prefix + "s-ch_" + channel + ".root");
	samples.push_back(s);
	s.ReSet();

	//for (auto iSample : samples) std::cout << NEventsExpected( iSample ,MCSelection) << std::endl;
	std::vector<double> EventsExpected;
	std::vector<int> Colors;
	std::vector<std::string> Labels;
	for (unsigned int iSample = 0; iSample < samples.size(); iSample ++)
	 {
		EventsExpected.push_back(NEventsExpected( samples[iSample] ,MCSelection));
		std::cout << samples[iSample].Processname << " & " << EventsExpected[iSample] << "\\\\" << std::endl;
		std::cout << "\\hline" << std::endl;
		Colors.push_back(samples[iSample].color);
		Labels.push_back(samples[iSample].Processname);
	 }

	 TPie *pie = new TPie("pie1","",4,&EventsExpected[0], &Colors[0]);
	 pie->SetLabelsOffset(0.0001);

	 //pie->SetLabelFormat("#splitline{%txt}");

	 for (unsigned int  iLabel = 0; iLabel < Labels.size(); iLabel ++) pie -> SetEntryLabel(iLabel, Labels[iLabel].c_str());
	 TCanvas *c1=  new TCanvas("c1","canvas",16000.,9000.);
	 pie -> Draw();
	 c1 -> SaveAs("WJets.png");

	 
}
