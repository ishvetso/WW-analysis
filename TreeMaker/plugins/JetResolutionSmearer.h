#include "JetMETCorrections/Modules/interface/JetResolution.h"

template<class T> class JetResolutionSmearer{
	JME::JetResolution resolution_pt;
	JME::JetResolutionScaleFactor resolution_sf;

	JetResolutionSmearer(){
		edm::FileInPath ResolutionFile_Pt("aTGCsAnalysis/TreeMaker/data/Fall15_25nsV2_MC_PtResolution_AK8PFchs.txt");
		edm::FileInPath ResolutionFile_Phi("aTGCsAnalysis/TreeMaker/data/Fall15_25nsV2_MC_PhiResolution_AK8PFchs.txt");
		resolution_pt = JME::JetResolution(ResolutionFile_Pt.fullPath());
	}
	double resolutionPt( T jet){
		JME::JetParameters parameters;
		parameters.setJetPt(jet.pt());
		parameters.setJetEta(jet.eta());
		double r = resolution_pt.getResolution(parameters);
		return r;
	}
	

};
