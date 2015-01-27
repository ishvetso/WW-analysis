// -*- C++ -*-
//
// Package:    ExoDiBosonResonances/GenHt_Analyser
// Class:      GenHt_Analyser
// 
/**\class GenHt_Analyser GenHt_Analyser.cc ExoDiBosonResonances/GenHt_Analyser/plugins/GenHt_Analyser.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ivan Shvetsov
//         Created:  Fri, 05 Dec 2014 11:47:44 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include "TTree.h"
#include "TFile.h"

//
// class declaration
//

class GenHt_Analyser : public edm::EDAnalyzer {
   public:
      explicit GenHt_Analyser(const edm::ParameterSet&);
      ~GenHt_Analyser();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      TTree* outTree_;
      //ht
      float Ht;

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
GenHt_Analyser::GenHt_Analyser(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree_ = fs->make<TTree>("EventInfo","EventInfo");
  outTree_->Branch("Ht", &Ht, "Ht/F");
}


GenHt_Analyser::~GenHt_Analyser()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenHt_Analyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   edm::Handle<edm::View<reco::Candidate> > genParticles;
   iEvent.getByLabel("prunedGenParticles", genParticles);

   //get Ht distribution, this is needed in order to check if merging of Wjets samples in different Ht bins is correct
   Ht = 0.0;
   for (unsigned int iGen = 0; iGen < genParticles -> size(); iGen++)
   {
      if( (genParticles -> at(iGen)).status() == 23 && ( fabs((genParticles -> at(iGen)).pdgId()) < 7 || ((genParticles -> at(iGen)).pdgId()) == 21 ) ) Ht += (genParticles -> at(iGen)).pt();
   }

    outTree_->Fill();
   
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
GenHt_Analyser::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenHt_Analyser::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
GenHt_Analyser::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
GenHt_Analyser::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
GenHt_Analyser::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
GenHt_Analyser::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenHt_Analyser::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenHt_Analyser);
