#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTreeFormula.h"
#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <algorithm> 
#include <Sample.cpp> 


double getNEvents(Sample sample_, bool isSignal=false)
{	

	TH1D * hist_temp  = new TH1D(sample_.Processname.c_str(), sample_.Processname.c_str(), 1, -10., 10.);
	hist_temp -> Sumw2();
	for (unsigned int iFile = 0; iFile < sample_.filenames.size(); iFile++)
 	{	
 		TFile file_(sample_.filenames.at(iFile).c_str(), "READ");
 		TTree *tree;
 		if(!isSignal) tree = (TTree*)file_.Get("BasicTree");
 		else  tree = (TTree*)file_.Get("treeDumper/BasicTree");
 		double charge_W_lep;
 		double totEventWeight;
 		std::vector<double> * aTGCWeights = 0;
 		if(!isSignal)tree -> SetBranchAddress("totEventWeight", &totEventWeight);
 		else tree -> SetBranchAddress("totWeight", &totEventWeight);
 		if (isSignal)tree -> SetBranchAddress("aTGCWeights", &aTGCWeights);
 		TTreeFormula *MCSampleSelection = new TTreeFormula("MCSampleSelection",sample_.selection.c_str(),tree);//that should be without any weights!
 		for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++)
 		{
 			tree -> GetEntry(iEntry);
 			if (MCSampleSelection -> EvalInstance() && !isSignal)hist_temp -> Fill(1., totEventWeight);
 			if (MCSampleSelection -> EvalInstance() && isSignal)hist_temp -> Fill(1., totEventWeight*(aTGCWeights->at(7))*2300./20);
 		}

 	}

 	double Nevents = hist_temp -> Integral();
 	return Nevents;
 	delete hist_temp;
}


void getSignalKFactor()
{
	string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_18April2016/";
	Sample  SM, Signal;
	std::string channel = "ele";
	string defaulCuts = "(jet_pt > 200. && jet_tau2tau1 < 0.6  && ( Mjpruned < 105. && Mjpruned > 65.) && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900. && nbtag == 0.";
	if (channel == "ele") defaulCuts += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") defaulCuts += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}

 	SM.SetParameters("SM", defaulCuts, kGreen);
 	SM.SetFileNames(prefix + "WW_" + channel + ".root");
 	SM.SetFileNames(prefix + "WZ_" + channel + ".root");

 	Signal.SetParameters("Signal", defaulCuts, kGreen);
 	Signal.SetFileNames(prefix + "WW-aTGC_" + channel + ".root");
 	Signal.SetFileNames(prefix + "WZ-aTGC_" + channel + ".root");
 	


	

 	double NeventsSignal  = getNEvents(Signal, true);
 	double NeventsSM  = getNEvents(SM);

 	std::cout << "channel : " << channel << " k factor " << NeventsSM/NeventsSignal << std::endl;

	
}



