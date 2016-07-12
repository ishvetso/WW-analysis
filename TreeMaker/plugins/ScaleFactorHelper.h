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
			edm::FileInPath SFFileMuID("aTGCsAnalysis/TreeMaker/data/MuonHighPt_Z_RunCD_Reco76X.root");
			TFile fileSFMuID(SFFileMuID.fullPath().c_str());
			hist_mu_iso = (TH2F *) fileSFMuID.Get("tkRelIsoIDPt53_PtEtaBins/pair_ne_ratio");
			hist_mu_iso -> SetDirectory(0);
			hist_mu_ID = (TH2F *) fileSFMuID.Get("HighPtIDPt53_PtEtaBins/pair_ne_ratio");
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
			if(hist_mu_SF -> GetXaxis() -> FindBin(pt) != hist_mu_SF -> GetXaxis() -> GetNbins() + 1 && pt < 200.){
				iBinPt = hist_mu_SF -> GetXaxis() -> FindBin(pt);
				iBinEta = hist_mu_SF -> GetYaxis() -> FindBin(eta);
				nom_SF = hist_mu_SF -> GetBinContent(iBinPt,iBinEta);
				error_SF = hist_mu_SF -> GetBinError(iBinPt,iBinEta);
			}
			else if (pt >= 200.){
				iBinEta = hist_mu_SF -> GetYaxis() -> FindBin(eta);
				iBinPt = hist_mu_SF -> GetNbinsX();
				nom_SF = hist_mu_SF -> GetBinContent(iBinPt,iBinEta);
				error_SF = hist_mu_SF -> GetBinError(iBinPt,iBinEta);
				
				if (variation=="down") error_SF += 0.025*nom_SF;//this follows the Zprime analysis: "So you should consider the SF at Z peak (in 76 they have been presented one month ago at muon POG) knowing that these SF can vary by -2.5%. In other word -2.5% is the systematic on your SF at Z peak to go to higher mass."
			

			}
			else throw cms::Exception("InvalidValue") << " out of range values for muon scale factors, please check: pt " << pt << " eta: " << eta << std::endl;
			if (type == "ID" && variation=="") SF = nom_SF;
			else if (type == "ID" && variation=="up") SF = nom_SF + error_SF;			
			else if (type == "ID" && variation=="down") SF = nom_SF - error_SF;
			else if (type == "trigger")	SF = Mu50::scaleFactor( pt, eta);
			else throw cms::Exception("InvalidValue") <<  " not supported type of scale factor is used !!! " << std::endl;	

			std::cout  << "type : "<< type  << " eta: " << eta << " pt " << pt << " SF " << SF <<  " error_SF : " << error_SF << std::endl;
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