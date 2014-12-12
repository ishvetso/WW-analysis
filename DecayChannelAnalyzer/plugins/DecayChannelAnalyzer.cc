// -*- C++ -*-
//
// Package:    ExoDiBosonResonances/DecayChannelAnalyzer
// Class:      DecayChannelAnalyzer
// 
/**\class DecayChannelAnalyzer DecayChannelAnalyzer.cc ExoDiBosonResonances/DecayChannelAnalyzer/plugins/DecayChannelAnalyzer.cc

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

#include "WW-analysis/TreeMaker/plugins/DecayChannel.h"
#include "WW-analysis/TreeMaker/plugins/DecayClass.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/View.h"

#include "TTree.h"
#include "TFile.h"

//
// class declaration
//

class DecayChannelAnalyzer : public edm::EDAnalyzer {
   public:
      explicit DecayChannelAnalyzer(const edm::ParameterSet&);
      ~DecayChannelAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      TTree* outTree_;
      //Decay Info (gen level)
      DecayClass WDecayClass;

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
DecayChannelAnalyzer::DecayChannelAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree_ = fs->make<TTree>("EventInfo","EventInfo");
  outTree_->Branch("WDecayClass", &WDecayClass, "WDecayClass/I");
}


DecayChannelAnalyzer::~DecayChannelAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
DecayChannelAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   edm::Handle<edm::View<reco::Candidate> > genParticles;
   iEvent.getByLabel("prunedGenParticles", genParticles);

   int N_had_Wgen  = 0, N_lep_Wgen = 0 ;
   DefineDecayChannel(genParticles, N_lep_Wgen , N_had_Wgen );
   
   if (N_lep_Wgen == 0 && N_had_Wgen == 2  ) WDecayClass = Hadronic;
   else if (N_lep_Wgen == 1 && N_had_Wgen == 1 ) WDecayClass = Semileptonic;
   else if (N_lep_Wgen == 2 && N_had_Wgen == 0 ) WDecayClass = Leptonic;
   else WDecayClass = UnDefined;

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
DecayChannelAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DecayChannelAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
DecayChannelAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
DecayChannelAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
DecayChannelAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
DecayChannelAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DecayChannelAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(DecayChannelAnalyzer);
