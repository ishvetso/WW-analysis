#ifndef  PLOTTER
#define PLOTTER

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
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>
#include <stdio.h>      
#include <stdlib.h>  
#include <Sample.hpp>
#include <var.hpp>
#include <TASImage.h>
#include <tdrstyle.hpp>
#include <CMS_lumi.hpp>
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
	std::map<std::pair<std::string, std::string>, Var> SystematicsVarMapUp, SystematicsVarMapDown;

	public:
	Plotter();	
	Plotter(CHANNEL channel_);
	Sample DataSample;
	Sample SignalSample;
	bool withData = true;
	void SetNbins(int Nbins_);
	void SetVar(vector <Var> variables_);
	void SetSamples(vector <Sample> samples_);	
	void SetDataSample(Sample sample_);
	void Plotting(std::string OutPrefix_);
	void PlottingVar(std::string OutPrefix_, Var var);
	//void Systematics(Var var, TH1D *& hist_nominal);
	

};

#endif
