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
	var.SetRange(0., 50.);
	variables.push_back(var);
	
	var.VarName = "l_pt";
	var.SetRange(60., 600.);
	variables.push_back(var);

	var.VarName = "l_eta";
	var.SetRange(-2.5, 2.5);
	variables.push_back(var);

	var.VarName = "l_phi";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "jet_pt";
	var.SetRange(200., 600.);
	variables.push_back(var);

	var.VarName = "jet_eta";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "jet_phi";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "W_pt";
	var.SetRange(60., 600.);
	variables.push_back(var);

	var.VarName = "W_eta";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "W_phi";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "pt_W_had";
	var.SetRange(60., 600.);
	variables.push_back(var);

	var.VarName = "eta_W_had";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "phi_W_had";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);


	var.VarName = "mass_W_lep";
	var.SetRange(30., 200.);
	variables.push_back(var);

	var.VarName = "mass_W_had";
	var.SetRange(30., 200.);
	variables.push_back(var);

	var.VarName = "mt_W_lep";
	var.SetRange(200., 600.);
	variables.push_back(var);

	var.VarName = "mt_W_had";
	var.SetRange(200., 600.);
	variables.push_back(var);

	var.VarName = "pfMET";
	var.SetRange(30., 600.);
	variables.push_back(var);



	var.VarName = "jet_mass_pruned";
	var.SetRange(0., 150.);
	variables.push_back(var);

	var.VarName = "jet_mass_softdrop";
	var.SetRange(0., 200.);
	variables.push_back(var);

	var.VarName = "jet_tau2tau1";
	var.SetRange(0., 1.1);
	variables.push_back(var);

	var.VarName = "m_lvj";
	var.SetRange(400., 1800.);
	variables.push_back(var);

	var.VarName = "njets";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "nbtag";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "nLooseEle";
	var.SetRange(0., 2.);
	variables.push_back(var);

	var.VarName = "nLooseMu";
	var.SetRange(0., 2.);
	variables.push_back(var);


	var.VarName = "NAK8jet";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "N_had_W";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "N_lep_W";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "charge_W_lep";
	var.SetRange(-2., 2.);
	variables.push_back(var);


	Plotter p;
	vector <Sample> samples;
	p.SetVar(variables);
	p.SetNbins(30);

	string addOnCutWjets = "(jet_mass_pruned < 65. || jet_mass_pruned > 95.)";
	string addOnCutTtbar = "(nbtag >= 1)";
	
	
	string MCSelection = "weight*" + addOnCutWjets;
	string DataSelection = addOnCutWjets;
		
	/*
	 * Colors
	 * kWhite =0,   kBlack =1,   kGray=920,  kRed   =632, kGreen =416, kBlue=600, 
	 * kYellow=400, kMagenta=616, kCyan=432, kOrange=800, kSpring=820, kTeal=840,
	 * kAzure =860, kViolet =880, kPink=900
	 */
	
	Sample s, dataSample;
	
	string prefix = "/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/test/tuples_50ns_29July2015/";
	
	s.SetParameters("WW", MCSelection, kRed);
 	s.SetFileNames( prefix + "WW_mu.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("W+jets", MCSelection, kGreen);
 	s.SetFileNames(prefix + "WJets_mu.root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("ttbar", MCSelection, kOrange);
 	s.SetFileNames(prefix + "ttbar_mu.root");
	samples.push_back(s);
	s.ReSet();

	dataSample.SetParameters("data", DataSelection, kBlack);
 	dataSample.SetFileNames(prefix + "data_mu_all.root");
	
	
	p.SetSamples(samples);
	p.DataSample = dataSample;
	p.withData = true;
 	p.Plotting("plots_ele_50ns_30July_WJets_enriched/");
	
	
}