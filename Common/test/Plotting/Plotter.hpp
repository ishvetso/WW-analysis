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
#include <thread>
#include <iostream>
#include <cmath>
#include <future>
#include <functional>
#include <TThread.h>
#include <stdio.h>      
#include <stdlib.h>  
#include <Sample.hpp>
#include <var.hpp>
#include <TASImage.h>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/tdrstyle.C>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/CMS_lumi.cpp>
#include <boost/algorithm/string/replace.hpp>
#include "boost/algorithm/string.hpp"
#include <mutex>
/*
 * Class to make data-MC comparison plots
 */

typedef unsigned int uint;
std::mutex PlotterMutex;
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
	Plotter();	
	Plotter(CHANNEL channel_);
	Sample DataSample;
	Sample SignalSample;
	bool withData = true;
	void SetNbins(int Nbins_);
	void SetVar(vector <Var> variables_);
	void SetSamples(vector <Sample> samples_);	
	void SetDataSample(Sample sample_);
	void GetHist(Sample sample_, Var var_, std::string TreeName, TH1D *& hist_);
	void GetHistFromSingleFile(std::string filename_, Var var_, Sample sample_, std::string TreeName, int Number, TH1D *& hist_);
	void GetHistThreaded(Sample sample_, Var var_, const  std::string & TreeName, TH1D *& hist_);
	
	void PlottingSingleVar(std::string OutPrefix_, Var var);
	void Systematics(Var var, TH1D *& hist_nominal);
	void PlotAllVars(std::string OutPrefix_);
	

};

#endif
