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


std::pair<double,double> getNEvents(Sample sample_, bool isMC=true)
{	

	TH1D * hist_temp  = new TH1D(sample_.Processname.c_str(), sample_.Processname.c_str(), 1, -10., 10.);
	hist_temp -> Sumw2();
	std::cout << hist_temp -> GetSumw2()-> GetAt(1) << std::endl;
	for (unsigned int iFile = 0; iFile < sample_.filenames.size(); iFile++)
 	{	
 		TFile file_(sample_.filenames.at(iFile).c_str(), "READ");
 		TTree *tree;
 		if(isMC) tree = (TTree*)file_.Get("BasicTree");
 		else  tree = (TTree*)file_.Get("treeDumper/BasicTree");
 		double charge_W_lep;
 		double totEventWeight;
 		if(isMC)tree -> SetBranchAddress("totEventWeight", &totEventWeight);
 		tree -> SetBranchAddress("charge_W_lep", &charge_W_lep);
 		TTreeFormula *MCSampleSelection = new TTreeFormula("MCSampleSelection",sample_.selection.c_str(),tree);//that should be without any weights!
 		for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++)
 		{
 			tree -> GetEntry(iEntry);
 			if (MCSampleSelection -> EvalInstance() && isMC)hist_temp -> Fill(charge_W_lep, totEventWeight);
 			if (MCSampleSelection -> EvalInstance() && !isMC)hist_temp -> Fill(charge_W_lep);
 		}

 	}

 	std::pair <double, double> Nevents;
 	Nevents.first = hist_temp -> Integral() ;
 	Nevents.second = sqrt(hist_temp -> GetSumw2()-> GetAt(1));
 	return Nevents;
 	delete hist_temp;
}


void calculateBTagVetoSF()
{
	string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_22July2016/";
	Sample ttbar, wjets,dataSample;
	string TTBarEnrichedBTagVeto = "(jet_pt > 200.  &&  jet_tau2tau1 < 0.6  && Mjpruned < 200. && Mjpruned > 155. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900. && nbtag == 0 ";
	std::string channel = "ele";
	
	if (channel == "ele") TTBarEnrichedBTagVeto += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") TTBarEnrichedBTagVeto += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}
	ttbar.SetParameters("ttbar", TTBarEnrichedBTagVeto, kOrange);
 	ttbar.SetFileNames(prefix + "ttbar-powheg-tot_"+ channel + ".root");

 	wjets.SetParameters("W+jets", TTBarEnrichedBTagVeto, kGreen);
 	wjets.SetFileNames(prefix + "WJets_HT-100To200-tot_" + channel + ".root");
 	wjets.SetFileNames(prefix + "WJets_HT-200To400-tot_" + channel + ".root");
 	wjets.SetFileNames(prefix + "WJets_HT-400To600_" + channel + ".root");
 	wjets.SetFileNames(prefix + "WJets_HT-600To800_" + channel + ".root");
 	wjets.SetFileNames(prefix + "WJets_HT-800To1200-tot_" + channel + ".root");
 	wjets.SetFileNames(prefix + "WJets_HT-1200To2500-tot_" + channel + ".root");
 	wjets.SetFileNames(prefix + "WJets_HT-2500ToInf-tot_" + channel + ".root");


	dataSample.SetParameters("data", TTBarEnrichedBTagVeto, kBlack);
 	dataSample.SetFileNames(prefix + "data-RunD_" + channel + ".root");

 	std::pair<double,double> ttbar_veto, wjets_veto, data_veto;
 	ttbar_veto = getNEvents(ttbar);
 	wjets_veto = getNEvents(wjets);
 	data_veto = getNEvents(dataSample,false);
 	
 

 	string TTBarEnrichedInclusive = "(jet_pt > 200.  &&  jet_tau2tau1 < 0.6  && Mjpruned < 200. && Mjpruned > 155. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900. ";
	if (channel == "ele") TTBarEnrichedInclusive += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") TTBarEnrichedInclusive += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}

	ttbar.SetParameters("ttbar1", TTBarEnrichedInclusive, kOrange);
	wjets.SetParameters("W+jets1", TTBarEnrichedInclusive, kGreen);
	dataSample.SetParameters("data1", TTBarEnrichedInclusive, kBlack);

	std::pair<double,double> ttbar_inclusive, wjets_inclusive, data_inclusive;
 	ttbar_inclusive = getNEvents(ttbar);
 	wjets_inclusive = getNEvents(wjets);
 	data_inclusive = getNEvents(dataSample, false);
 	double SF_wjets = (data_veto.first - wjets_veto.first)/(data_inclusive.first - wjets_inclusive.first) ;
 	double SF_ttbar = ttbar_veto.first/ttbar_inclusive.first;
 	std::cout << SF_wjets/SF_ttbar << std::endl;
 	
 	double SF_wjets_num = (data_veto.first - wjets_veto.first);
 	double SF_wjets_denom = (data_inclusive.first - wjets_inclusive.first);
 	double SF_wjets_error_num = sqrt(pow(data_veto.second,2) + pow(wjets_veto.second,2)  );
 	double SF_wjets_error_denom = sqrt(pow(data_inclusive.second,2) + pow(wjets_inclusive.second,2)  );
 	double SF_wjets_error = SF_wjets*sqrt(pow(SF_wjets_error_num/SF_wjets_num, 2 ) + pow(SF_wjets_error_denom/SF_wjets_denom, 2 ) - 2/(SF_wjets_denom*SF_wjets_num));

	double SF_ttbar_num = ttbar_veto.first;
 	double SF_ttbar_denom = ttbar_inclusive.first;
 	double SF_ttbar_error_num = ttbar_veto.second;
 	double SF_ttbar_error_denom = ttbar_inclusive.second ;
 	double SF_ttbar_error = SF_ttbar*sqrt(pow(SF_ttbar_error_num/SF_ttbar_num, 2 ) + pow(SF_ttbar_error_denom/SF_ttbar_denom, 2 ) - 2/(SF_ttbar_denom*SF_ttbar_num)); 	

 	double SF = SF_wjets/SF_ttbar;
 	double totSFerr = SF*sqrt(pow(SF_wjets_error/SF_wjets, 2) + pow(SF_ttbar_error/SF_ttbar, 2));
 	std::cout<< "SF: " << SF << " totSFerr :" << totSFerr << std::endl;
 	std::cout<< "data_veto: " << data_veto.first << " err :" << data_veto.second << std::endl;
 	std::cout<< "data_inclusive: " << data_inclusive.first << " err :" << data_inclusive.second << std::endl;
 	std::cout<< "ttbar_veto: " << ttbar_veto.first << " err :" << ttbar_veto.second << std::endl;
 	std::cout<< "ttbar_inclusive: " << ttbar_inclusive.first << " err :" << ttbar_inclusive.second << std::endl;
 	std::cout<< "wjets_veto: " << wjets_veto.first << " err :" << wjets_veto.second << std::endl;
 	std::cout<< "wjets_inclusive: " << wjets_inclusive.first << " err :" << wjets_inclusive.second << std::endl;

 	
}



