#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include <random>

template<class T> class JetResolutionSmearer{
	bool isMC;
	JME::JetResolution resolution_pt;
	JME::JetResolution resolution_phi;
	JME::JetResolutionScaleFactor resolution_sf;
	double rho;
	std::default_random_engine generator;
public:
	JetResolutionSmearer(bool isMC_){
		isMC = isMC_;
		std::string prefix  = "aTGCsAnalysis/TreeMaker/data/";
		edm::FileInPath ResolutionFile_Pt(prefix + (isMC?"Fall15_25nsV2_MC_PtResolution_AK8PFchs.txt":"Fall15_25nsV2_DATA_PtResolution_AK8PFchs.txt"));
		edm::FileInPath ResolutionFile_Phi(prefix + (isMC?"Fall15_25nsV2_MC_PhiResolution_AK8PFchs.txt":"Fall15_25nsV2_DATA_PhiResolution_AK8PFchs.txt"));
		edm::FileInPath SF_File(prefix + (isMC?"Fall15_25nsV2_MC_SF_AK8PFchs.txt":"Fall15_25nsV2_DATA_SF_AK8PFchs.txt"));
		resolution_pt = JME::JetResolution(ResolutionFile_Pt.fullPath());
		resolution_phi = JME::JetResolution(ResolutionFile_Phi.fullPath());
		resolution_sf = JME::JetResolutionScaleFactor(SF_File.fullPath());
	}
	void setRho(double rho_){rho = rho;};
	double resolutionPt( T jet){
		JME::JetParameters parameters;
		parameters.setJetPt(jet.pt());
		parameters.setJetEta(jet.eta());
		parameters.setRho(rho);
		double r = resolution_pt.getResolution(parameters);
		return r;
	}
	double resolutionPhi( T jet){
		JME::JetParameters parameters;
		parameters.setJetPt(jet.pt());
		parameters.setJetEta(jet.eta());
		parameters.setRho(rho);
		double r = resolution_phi.getResolution(parameters);
		return r;
	}
	double ScaleFactor(T jet){
		JME::JetParameters parameters = {{JME::Binning::JetEta, jet.eta()}};
		float sf = resolution_sf.getScaleFactor(parameters);
		return sf;
	}
	double smearedPt(T jet){
		double resolutionPt_ = resolutionPt(jet);
		double SF = ScaleFactor(jet);
		double width = resolutionPt_ * sqrt(SF*SF - 1.);
  		std::normal_distribution<double> distribution(jet.pt(),width);
  		double pt = distribution(generator);
  		return pt;

	}
	double smearedPhi(T jet){
		double resolutionPhi_ = resolutionPhi(jet);
		double SF = ScaleFactor(jet);
		double width = resolutionPhi_ * sqrt(SF*SF - 1.);
  		std::normal_distribution<double> distribution(jet.phi(),width);
  		double phi = distribution(generator);
  		return phi;

	}

};
