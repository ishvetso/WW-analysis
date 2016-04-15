#include "TH2F.h"
#include <iostream>

/*
* This macro supports application of scale factors in the analysis
* electron channel is not covered
* Author: Ivan Shvetsov, 2016
*/

namespace sf
{
	edm::FileInPath SFFileMuID("aTGCsAnalysis/TreeMaker/data/MuonHighPt_Z_RunCD_Reco74X_Dec17.root");
	TFile fileSFMuID(SFFileMuID.fullPath().c_str());
	TH2F *hist_mu_ID = (TH2F *) fileSFMuID.Get("tkRelIsoID_PtEtaBins_Pt20/pTtuneP_abseta_ratio");

	edm::FileInPath SFFileMuTrigger("aTGCsAnalysis/TreeMaker/data/SingleMuonTrigger_Z_RunCD_Reco76X_Feb15-2.root");
	TFile fileSFMuTrigger(SFFileMuTrigger.fullPath().c_str());
	TH2F *hist_mu_trigger = (TH2F *) fileSFMuTrigger.Get("runD_Mu45_eta2p1_PtEtaBins/pt_abseta_ratio");

	double getScaleFactor(double pt, double eta, std::string channel, std::string type){

		double SF;
		if (channel == "mu")
		{
			if (type == "ID"){
				if(hist_mu_ID -> GetXaxis() -> FindBin(pt) != hist_mu_ID -> GetXaxis() -> GetNbins() + 1) SF = hist_mu_ID -> GetBinContent(hist_mu_ID -> GetXaxis() -> FindBin(pt), hist_mu_ID -> GetYaxis() -> FindBin(eta));
				else SF = hist_mu_ID -> GetBinContent(hist_mu_ID -> GetXaxis() -> GetNbins(), hist_mu_ID -> GetYaxis() -> FindBin(eta));
			}
			else if (type == "trigger"){
				if(hist_mu_trigger -> GetXaxis() -> FindBin(pt) != hist_mu_trigger -> GetXaxis() -> GetNbins() + 1) SF = hist_mu_trigger -> GetBinContent(hist_mu_trigger -> GetXaxis() -> FindBin(pt), hist_mu_trigger -> GetYaxis() -> FindBin(eta));
				else SF = hist_mu_trigger -> GetBinContent(hist_mu_trigger -> GetXaxis() -> GetNbins(), hist_mu_trigger -> GetYaxis() -> FindBin(eta));
			}
			else {
				throw cms::Exception("InvalidValue") <<  " not supported type of scale factor is used !!! " << std::endl;	
			}
		}

		//electron is not yet supported
		else if (channel == "el")
		{
			if (type == "ID"){
				SF = 1.;
			}
			else if (type == "ID"){
				SF = 1.;
			}

			else {
				throw cms::Exception("InvalidValue") <<  " not supported type of scale factor is used !!! " << std::endl;	
			}
		}

		else throw cms::Exception("InvalidValue") <<  " not supported channel is used !!! " << std::endl;	

		if (SF == 0.) {
			throw cms::Exception("InvalidValue") << " pt  : " << pt << " eta : " << eta <<  ". Lepton scale factor is zero !!! " << std::endl;
		}
		return SF;
	}
}