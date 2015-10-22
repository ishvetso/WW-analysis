// -*- C++ -*-
//
// Package:    WW-analysis/WLepLorentzVectorSystematics
// Class:      WLepLorentzVectorSystematics
// 
/**\class WLepLorentzVectorSystematics WLepLorentzVectorSystematics.cc WW-analysis/WLepLorentzVectorSystematics/plugins/WLepLorentzVectorSystematics.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ivan Shvetsov
//         Created:  Thu, 15 Oct 2015 07:50:44 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/MET.h"     
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include <map>
#include <utility>
#include <string>
#include <vector>


//
// class declaration
//

class WLepSystematicsHelper {
	std::vector< edm::EDGetTokenT<std::vector<reco::CompositeCandidate> > > tokens;
	std::vector<std::string> ListOfSystematics;
public:
    WLepSystematicsHelper(std::string channel, edm::ConsumesCollector iC){
	ListOfSystematics.push_back("UnclusteredEn");
	ListOfSystematics.push_back("JetEn");
  ListOfSystematics.push_back("LeptonEn");
	tokens.resize(2 * ListOfSystematics.size());
     int iSystUp, iSystDown;
     for (unsigned int iSyst = 0; iSyst < ListOfSystematics.size(); iSyst ++) {	
     	iSystUp = 2*iSyst;
     	iSystDown = 2*iSyst + 1;
      if (ListOfSystematics.at(iSyst) != "LeptonEn"){
        tokens.at(iSystUp)  =  iC.mayConsume<std::vector<reco::CompositeCandidate>>(edm::InputTag("Wto" + channel + "nu" + ListOfSystematics.at(iSyst) + "Up"));
        tokens.at(iSystDown) = iC.mayConsume<std::vector<reco::CompositeCandidate>>(edm::InputTag("Wto" + channel + "nu" + ListOfSystematics.at(iSyst) + "Down"));
      }
      else{
          std::string name;
          if ( channel == "el") name = "ElectronEn";
          else if ( channel == "mu") name = "MuonEn";
          else std::cerr << "Invalid channel, use el or mu" <<  std::endl;
          tokens.at(iSystUp)  =  iC.mayConsume<std::vector<reco::CompositeCandidate>>(edm::InputTag("Wto" + channel + "nu" + name + "Up"));
          tokens.at(iSystDown) = iC.mayConsume<std::vector<reco::CompositeCandidate>>(edm::InputTag("Wto" + channel + "nu" + name + "Down"));
      }
	 }
}

   std::map<std::string, math::XYZTLorentzVector> getWLepSystematicsLoretzVectors(const edm::Event& iEvent){
   std::vector<edm::Handle<std::vector<reco::CompositeCandidate> > > WLeps;
   WLeps.resize(2 * ListOfSystematics.size());
   
   std::map<std::string, math::XYZTLorentzVector>  SystMap;

   //loop over available systematics, not everything is supported at the moment!
   for (unsigned int iSyst = 0; iSyst < ListOfSystematics.size(); iSyst ++) {	
   	
   	//get objects from the event
   	iEvent.getByToken(tokens.at( 2*iSyst), WLeps.at(2*iSyst));
   	iEvent.getByToken(tokens.at( 2*iSyst + 1), WLeps.at(2*iSyst + 1));

   	//if there is leptonically decaying W get 4-vecton and insert into map 
   	if (WLeps.at(2*iSyst) -> size() > 0 && WLeps.at(2*iSyst + 1) -> size() > 0) {
   		reco::CompositeCandidate WLepSystUp = (WLeps.at(2*iSyst)) -> at(0); 
   		reco::CompositeCandidate WLepSystDown = (WLeps.at(2*iSyst + 1)) -> at(0); 
   		math::XYZTLorentzVector P4Up = WLepSystUp.p4();
   		math::XYZTLorentzVector P4Down = WLepSystDown.p4();
   		SystMap.insert( std::pair<std::string,math::XYZTLorentzVector>(ListOfSystematics.at(iSyst) + "Up",P4Up) );
   		SystMap.insert( std::pair<std::string,math::XYZTLorentzVector>(ListOfSystematics.at(iSyst) + "Down",P4Down) );
  	}
   }
   


   	return SystMap;


   }
   
};