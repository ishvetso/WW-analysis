// -*- C++ -*-
//
// Package:    WW-analysis/LeptonVeto
// Class:      LeptonVeto
// 
/**\class LeptonVeto LeptonVeto.cc WW-analysis/LeptonVeto/plugins/LeptonVeto.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ivan Shvetsov
//         Created:  Thu, 25 Jun 2015 15:55:04 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CompositeCandidateFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class LeptonVeto : public edm::EDFilter {
   public:
      explicit LeptonVeto(const edm::ParameterSet&);
      ~LeptonVeto();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      edm::EDGetTokenT<edm::View<reco::Candidate> > looseLeptonToken_;
      edm::EDGetTokenT<edm::View<reco::Candidate> > tightLeptonToken_;
      int minNLoose, maxNLoose, minNTight, maxNTight;
      
      //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
      //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
      //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

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
LeptonVeto::LeptonVeto(const edm::ParameterSet& iConfig):
   looseLeptonToken_(consumes<edm::View<reco::Candidate> > (iConfig.getParameter<edm::InputTag>( "looseLeptonSrc" ) ) ),
   tightLeptonToken_(consumes<edm::View<reco::Candidate> > (iConfig.getParameter<edm::InputTag>( "tightLeptonSrc" ) ) ),
   minNLoose(iConfig.getParameter<int>( "minNLoose" )  ),
   maxNLoose(iConfig.getParameter<int>( "maxNLoose" )  ),
   minNTight(iConfig.getParameter<int>( "minNTight" )  ),
   maxNTight(iConfig.getParameter<int>( "maxNTight" )  )
{
   //now do what ever initialization is needed

}


LeptonVeto::~LeptonVeto()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
LeptonVeto::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   
   Handle <edm::View<reco::Candidate> > looseLeptons;
   Handle <edm::View<reco::Candidate> > tightLeptons;
   
    iEvent.getByToken(looseLeptonToken_, looseLeptons);
    iEvent.getByToken(tightLeptonToken_, tightLeptons);
    
    int nLoose, nTight;
    
    nLoose = looseLeptons -> size();
    nTight = tightLeptons -> size();
    
    bool pass = (minNLoose <= nLoose) && (nLoose <= maxNLoose ) && (minNTight <= nTight) && (nTight <= maxNTight ) ;
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
   return pass;
}

// ------------ method called once each job just before starting event loop  ------------
void 
LeptonVeto::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
LeptonVeto::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
LeptonVeto::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
LeptonVeto::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
LeptonVeto::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
LeptonVeto::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
LeptonVeto::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(LeptonVeto);
