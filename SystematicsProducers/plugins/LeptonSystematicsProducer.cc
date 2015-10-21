// -*- C++ -*-
//
// Package:    WW-analysis/LeptonSystematicsProducer
// Class:      LeptonSystematicsProducer
// 
/**\class LeptonSystematicsProducer LeptonSystematicsProducer.cc WW-analysis/LeptonSystematicsProducer/plugins/LeptonSystematicsProducer.cc

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
#include <vector>     

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"     
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"     

//
// class declaration
//

class LeptonSystematicsProducer : public edm::EDProducer {
   public:
      explicit LeptonSystematicsProducer(const edm::ParameterSet&);
      ~LeptonSystematicsProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual void produce(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      edm::EDGetTokenT<edm::View<reco::Candidate> > leptonsToken;
      std::string channel;
      std::string variation;
      

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
LeptonSystematicsProducer::LeptonSystematicsProducer(const edm::ParameterSet& iConfig):
  variation(iConfig.getParameter<std::string>("variation"))
{
   channel = iConfig.getParameter<std::string>("channel");  
   leptonsToken = mayConsume<edm::View<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("leptonSource"));
   produces<std::vector<reco::LeafCandidate>>();
  
  
}


LeptonSystematicsProducer::~LeptonSystematicsProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
LeptonSystematicsProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   edm::Handle<edm::View<reco::Candidate> > leptons;
   iEvent.getByToken(leptonsToken, leptons);
   
   std::auto_ptr<std::vector<reco::LeafCandidate>> outCollection(new std::vector<reco::LeafCandidate> );

   reco::LeafCandidate lepton;
   lepton = (reco::LeafCandidate)leptons->at(0);


   
  
   //set the lepton uncertainty
   double LeptonUncertainty =0.;
   if (channel == "el") LeptonUncertainty = 0.001;//this  uncertainty is set temporary (after talking with Lindsey), not yet retrieved for 13 TeV
   else if (channel == "mu")  {
    LeptonUncertainty = 0.002;// 13 TeV uncertainty not yet available, use recommendation from Run I: if pt of muon < 200, uncertainty is 0.2%, else the uncertainty on the 1/pT bias is about 0.05 c/TeV : https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceResolution#General_recommendations_for_muon
    if (lepton.pt() > 200. ) {
           double extraEnUncertainty = (lepton.pt()/1000.)*0.05;
           LeptonUncertainty = sqrt(pow(LeptonUncertainty,2) + pow(extraEnUncertainty,2));
    }
   }
   else std::cerr << "Invalid channel used, use el or mu" << std::endl;

    math::XYZTLorentzVector SmearedP4;   
   if (variation == "up") SmearedP4.SetPxPyPzE((1 + LeptonUncertainty)*(lepton.px()),(1 + LeptonUncertainty)*(lepton.py()), lepton.pz()  ,(1 + LeptonUncertainty)*(lepton.energy()) ) ;
   else if (variation == "down") SmearedP4.SetPxPyPzE((1 - LeptonUncertainty)*(lepton.px()),(1 - LeptonUncertainty)*(lepton.py()), lepton.pz()  ,(1 - LeptonUncertainty)*(lepton.energy()) ) ;
   else std::cerr << "Invalid variation used in systematics, use up or down.";
   lepton.setP4(SmearedP4);

   outCollection -> push_back(lepton);
  
   iEvent.put(outCollection);
     
}

// ------------ method called once each job just before starting event loop  ------------
void 
LeptonSystematicsProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LeptonSystematicsProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
LeptonSystematicsProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
LeptonSystematicsProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
LeptonSystematicsProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
LeptonSystematicsProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LeptonSystematicsProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(LeptonSystematicsProducer);
