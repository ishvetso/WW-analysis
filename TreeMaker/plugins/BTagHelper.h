#include "aTGCsAnalysis/TreeMaker/interface/BTagCalibrationStandalone.h"

template<class T> class BTagHelper{

	BTagCalibration calib;
	float MaxBJetPt;
public:
	std::auto_ptr<BTagCalibrationReader>  reader;
	BTagHelper()
	{
		edm::FileInPath CSVFile("aTGCsAnalysis/TreeMaker/data/CSVv2.csv");
		calib = BTagCalibration("CSVv2", CSVFile.fullPath());
		reader.reset(new BTagCalibrationReader(BTagEntry::OP_TIGHT, "central"));
		reader->load(calib, BTagEntry::FLAV_B, "mujets");
		MaxBJetPt = 670.;
	}
	double getScaleFactor(T jet){
		float jetPt = jet.pt();
      	if (jetPt>MaxBJetPt) jetPt = MaxBJetPt;  
		 double jet_scalefactor = reader->eval(BTagEntry::FLAV_B, jet.eta(), jetPt);
		 return jet_scalefactor;
	}

};