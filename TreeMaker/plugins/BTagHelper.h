#include "CondFormats/BTauObjects/interface/BTagCalibration.h"
#include "CondFormats/BTauObjects/interface/BTagCalibrationReader.h"

template<class T> class BTagHelper{

	BTagCalibration calib;
	BTagCalibrationReader reader, reader_up, reader_down;
	float MaxBJetPt = 670.;
public:
	BTagHelper():
	reader(&calib, BTagEntry::OP_TIGHT,"mujets","central"),
	reader_up(&calib, BTagEntry::OP_TIGHT,"mujets","up"),
	reader_down(&calib, BTagEntry::OP_TIGHT,"mujets","down"){
		edm::FileInPath CSVFile("aTGCsAnalysis/TreeMaker/data/CSVv2.csv");
		calib = BTagCalibration("CSVv2", CSVFile.fullPath());
	}
	double getScaleFactor(T jet){
		float jetPt = jet.pt();
      	if (jetPt>MaxBJetPt) jetPt = MaxBJetPt;  
		 double jet_scalefactor = reader.eval(BTagEntry::FLAV_B, jet.eta(), jetPt); 
		 return jet_scalefactor;
	}

};