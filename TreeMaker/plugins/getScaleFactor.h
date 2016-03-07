#include "TH2F.h"
#include <iostream>

edm::FileInPath SFFile("aTGCsAnalysis/TreeMaker/data/MuonHighPt_Z_RunCD_Reco74X_Dec17.root");
TFile fileSF(SFFile.fullPath().c_str());
TH2F *hist = (TH2F *) fileSF.Get("tkRelIsoID_PtEtaBins_Pt20/pTtuneP_abseta_ratio");

double getScaleFactor(double pt, double eta){

	double SF;
	if(hist -> GetXaxis() -> FindBin(pt) != hist -> GetXaxis() -> GetNbins() + 1) SF = hist -> GetBinContent(hist -> GetXaxis() -> FindBin(pt), hist -> GetYaxis() -> FindBin(eta));
	else SF = hist -> GetBinContent(hist -> GetXaxis() -> GetNbins(), hist -> GetYaxis() -> FindBin(eta));

	if (SF == 0.) throw cms::Exception("InvalidValue") << "Lepton scale factor is zero !!! " << std::endl;
	return SF;
}