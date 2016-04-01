// -*- C++ -*-
//
// Package:    ExoDiBosonResonances/GenWeights
// Class:      GenWeights
// 
/**\class GenWeights GenWeights.cc ExoDiBosonResonances/GenWeights/plugins/GenWeights.cc
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
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"  
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"   
#include "boost/algorithm/string.hpp"


#include "TTree.h"
#include "TFile.h"

//
// class declaration
//

class GenWeights : public edm::EDAnalyzer {
   public:
      explicit GenWeights(const edm::ParameterSet&);
      ~GenWeights();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      TTree* outTree;
      
      edm::EDGetTokenT<GenEventInfoProduct> genInfoToken;
      double genWeight;
      

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
GenWeights::GenWeights(const edm::ParameterSet& iConfig):
genInfoToken(consumes<GenEventInfoProduct> (iConfig.getParameter<edm::InputTag>( "genInfo" ) ))
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree = fs->make<TTree>("Tree","Tree");
  outTree->Branch("genWeight",       &genWeight,        "genWeight/D"           );
}


GenWeights::~GenWeights()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenWeights::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle <GenEventInfoProduct> genInfo;
   iEvent.getByToken(genInfoToken, genInfo); 
   genWeight = (genInfo -> weight());
   outTree -> Fill();


   
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
GenWeights::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenWeights::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
GenWeights::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
GenWeights::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
GenWeights::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
GenWeights::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenWeights::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenWeights);
