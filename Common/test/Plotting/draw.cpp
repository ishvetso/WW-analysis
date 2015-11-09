#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TH1.h>
#include <THStack.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <iostream>
#include <iostream>
#include <sstream>
#include <map>
#include "Plotter.cpp"

void draw()
{
	vector <Var> variables;
	Var var;

	var.VarName = "nPV";
	var.Title = "n_{PV}";
	var.SetRange(0., 30.);
	variables.push_back(var);
	
	var.VarName = "l_pt";
	var.Title = "p_{T, lepton}";
	var.SetRange(60., 800.);
	variables.push_back(var);

	var.VarName = "l_eta";
	var.Title = "#eta_{lepton}";
	var.SetRange(-2.5, 2.5);
	variables.push_back(var);

	var.VarName = "l_phi";
	var.Title = "#phi_{lepton}";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "jet_pt";
	var.Title = "p_{T, jet}";
	var.SetRange(200., 1000.);
	variables.push_back(var);

	var.VarName = "jet_eta";
	var.Title = "#eta_{jet}";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "jet_phi";
	var.Title = "#phi_{lepton}";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "W_pt";
	var.Title = "p_{T, W}";
	var.SetRange(60., 1000.);
	variables.push_back(var);

	var.VarName = "W_eta";
	var.Title = "#eta_{W}";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "W_phi";
	var.Title = "#phi_{W}";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	


	var.VarName = "mass_W_lep";
	var.Title = "m_{W lep}";
	var.SetRange(30., 1500.);
	variables.push_back(var);

	var.VarName = "mt_W_lep";
	var.Title = "m_{T, W lep}";
	var.SetRange(200., 1100.);
	variables.push_back(var);

	var.VarName = "pfMET";
	var.Title = "MET";
	var.SetRange(30., 900.);
	variables.push_back(var);

	var.VarName = "pfMETPhi";
	var.Title = "#phi(MET)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);



	var.VarName = "jet_mass_pruned";
	var.Title = "m_{jet pruned}";
	var.SetRange(0., 400.);
	variables.push_back(var);

	var.VarName = "jet_mass_softdrop";
	var.Title = "m_{jet softdrop}";
	var.SetRange(0., 400.);
	variables.push_back(var);

	var.VarName = "jet_tau2tau1";
	var.Title = "#tau_{21}";
	var.SetRange(0., 1.1);
	variables.push_back(var);

	var.VarName = "m_lvj";
	var.Title = "m_{WV}";
	var.SetRange(400., 2500.);
	variables.push_back(var);

	var.VarName = "njets";
	var.Title = "n_{jets}";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "nbtag";
	var.Title = "n_{btag}";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "nLooseEle";
	var.Title = "nLooseEle";
	var.SetRange(0., 2.);
	variables.push_back(var);

	var.VarName = "nLooseMu";
	var.Title = "nLooseMu";
	var.SetRange(0., 2.);
	variables.push_back(var);


	var.VarName = "NAK8jet";
	var.Title = "n_{fat jets}";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "N_had_W";
	var.Title = "N_{had W}";
	var.SetRange(-1., 6.);
	variables.push_back(var);

	var.VarName = "N_lep_W";
	var.Title = "N_{lep W}";
	var.SetRange(-1., 6.);
	variables.push_back(var);

	var.VarName = "charge_W_lep";
	var.Title = "charge(W lep)";
	var.SetRange(-2., 2.);
	variables.push_back(var);

	var.VarName = "deltaR_LepWJet";
	var.Title = "#DeltaR(Lep, WJet)";
	var.SetRange(0., 10.);
	variables.push_back(var);

	var.VarName = "deltaPhi_LepMet";
	var.Title = "#Delta#phi(Lep, MET)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);

	var.VarName = "deltaPhi_WJetMet";
	var.Title = "#Delta#phi(WJet, MET)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);

	var.VarName = "deltaPhi_WJetWlep";
	var.Title = "#Delta#phi(WJet, WLep)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);


	Plotter p;
	vector <Sample> samples;
	p.SetVar(variables);
	p.SetNbins(30);

	string defaulCuts = "(jet_pt > 200.)";
	string addOnCutWjets = defaulCuts +  " * (jet_mass_pruned < 65. || jet_mass_pruned > 95.)";
	string addOnCutTtbar = defaulCuts +  " * (nbtag >= 1)";
	
	
	string MCSelection = "weight*PUweight*" + addOnCutWjets;
	string DataSelection = addOnCutWjets;
	std::string channel = "mu";
		
	/*
	 * Colors
	 * kWhite =0,   kBlack =1,   kGray=920,  kRed   =632, kGreen =416, kBlue=600, 
	 * kYellow=400, kMagenta=616, kCyan=432, kOrange=800, kSpring=820, kTeal=840,
	 * kAzure =860, kViolet =880, kPink=900
	 */
	
	Sample s, dataSample;
	
	string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_5November2015/";
	
	s.SetParameters("WW", MCSelection, kRed);
 	s.SetFileNames( prefix + "WW_"+ channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WZ", MCSelection, kRed-7);
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

	s.SetParameters("Single Top", MCSelection, kBlue);
 	s.SetFileNames(prefix + "t-ch_top_" + channel + ".root");
 	s.SetFileNames(prefix + "t-ch_antitop_" + channel + ".root");
 	s.SetFileNames(prefix + "tW-ch_" + channel + ".root");
 	s.SetFileNames(prefix + "s-ch_" + channel + ".root");
	samples.push_back(s);
	s.ReSet();

	dataSample.SetParameters("data", DataSelection, kBlack);
 	dataSample.SetFileNames(prefix + "data_05Oct_L2L3_" + channel + ".root");
 	dataSample.SetFileNames(prefix + "data_Prompt_L2L3_" + channel + ".root");
	
	
	p.SetSamples(samples);
	p.DataSample = dataSample;
	p.withData = true;
 	p.Plotting("plots_25ns_5November2015_PU_reweighting_L2L3_corrected_mu/");
	
	
}