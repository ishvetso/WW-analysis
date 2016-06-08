#include "TH2F.h"
#include <iostream>
#include "Mu50.h"

/*
* This class supports application of scale factors in the analysis
* electron channel is not covered
* Author: Ivan Shvetsov, 2016
*/


class ScaleFactorHelper{
	
	TH2F * hist_mu_iso;
	TH2F * hist_mu_ID;
	TH2F * hist_mu_SF;

public:
	ScaleFactorHelper(std::string channel){

		if (channel == "mu"  ){
			edm::FileInPath SFFileMuID("aTGCsAnalysis/TreeMaker/data/MuonHighPt_Z_RunCD_Reco74X_Dec17.root");
			TFile fileSFMuID(SFFileMuID.fullPath().c_str());
			hist_mu_iso = (TH2F *) fileSFMuID.Get("tkRelIsoID_PtEtaBins_Pt53/pTtuneP_abseta_ratio");
			hist_mu_iso -> SetDirectory(0);
			hist_mu_ID = (TH2F *) fileSFMuID.Get("HighPtID_PtEtaBins_Pt53/pTtuneP_abseta_ratio");
			hist_mu_iso -> SetDirectory(0);
			hist_mu_SF = (TH2F*)hist_mu_iso->Clone();
			hist_mu_SF -> SetDirectory(0);
			hist_mu_SF -> Multiply(hist_mu_ID);
		}
	}
	

    double getScaleFactor(double pt, double eta, std::string channel, std::string type, std::string variation=""){

		double SF;
		if (channel == "mu")
		{		

			int iBinPt, iBinEta;
			double nom_SF;
			double error_SF;
			if(hist_mu_SF -> GetXaxis() -> FindBin(pt) != hist_mu_SF -> GetXaxis() -> GetNbins() + 1){
				iBinPt = hist_mu_SF -> GetXaxis() -> FindBin(pt);
				//else iBinPt = hist_mu_SF -> GetXaxis() -> GetNbins();
				iBinEta = hist_mu_SF -> GetYaxis() -> FindBin(eta);
				nom_SF = hist_mu_SF -> GetBinContent(iBinPt,iBinEta);
				error_SF = hist_mu_SF -> GetBinError(iBinPt,iBinEta);
			}
			else if (pt >= 300.){
				nom_SF = 0.975;
				if (variation=="up") error_SF = 0.;
				else if (variation=="down") error_SF = 0.975 - 0.975*0.975;
				else error_SF = 0.;

			}
			else throw cms::Exception("InvalidValue") << " out of range values for muon scale factors, please check: pt " << pt << " eta: " << eta << std::endl;
			if (type == "ID" && variation=="") SF = nom_SF;
			else if (type == "ID" && variation=="up") SF = nom_SF + error_SF;			
			else if (type == "ID" && variation=="down") SF = nom_SF - error_SF;
			else if (type == "trigger")	SF = Mu50::scaleFactor( pt, eta);
			else throw cms::Exception("InvalidValue") <<  " not supported type of scale factor is used !!! " << std::endl;	
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
			throw cms::Exception("InvalidValue") << " pt  : " << pt << " eta : " << eta <<  ". Lepton " <<  type << " scale factor is zero !!! " << std::endl;
		}
		return SF;
	}
};