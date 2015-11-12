#ifndef  PLOTTER
#define PLOTTER

#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TH1.h>
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
#include <Sample.hpp>
#include <var.hpp>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/tdrstyle.C>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/CMS_lumi.cpp>
#include <boost/algorithm/string/replace.hpp>
#include "boost/algorithm/string.hpp"

/*
 * Class to make data-MC comparison plots
 */

typedef unsigned int uint;

enum CHANNEL {
	ELECTRON,	MUON
};

class Plotter
{
    int Nbins;
	vector <Sample> samples;
	CHANNEL channel;
	
	vector <Var> variables;

	public:
	Plotter(CHANNEL channel_);
	Sample DataSample;
	bool withData = true;
	void SetNbins(int Nbins_);
	void SetVar(vector <Var> variables_);
	void SetSamples(vector <Sample> samples_);	
	void SetDataSample(Sample sample_);
	void Plotting(std::string OutPrefix_);
	void Systematics(Var var, TH1D * hist_nominal);

};

#endif