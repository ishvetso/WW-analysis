// -*- C++ -*-
//
// Package:    WW-analysis/PUTrueDistProducer
// Class:      PUTrueDistProducer
// 
/**\class PUTrueDistProducer PUTrueDistProducer.cc WW-analysis/PUTrueDistProducer/plugins/PUTrueDistProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ivan Shvetsov
//         Created:  Mon, 12 Oct 2015 08:13:08 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <TH1F.h>
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class PUTrueDistProducer : public edm::EDAnalyzer {
   public:
      explicit PUTrueDistProducer(const edm::ParameterSet&);
      ~PUTrueDistProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      TH1F * pileupHist;

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
PUTrueDistProducer::PUTrueDistProducer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  pileupHist  = fs->make<TH1F>("pileup", "pileup", 50, 0., 50.);
  pileupHist -> Sumw2();

}


PUTrueDistProducer::~PUTrueDistProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
PUTrueDistProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  float Tnpv = -1;

  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

   int BX = PVI->getBunchCrossing();

   if(BX == 0) { 
     Tnpv = PVI->getTrueNumInteractions();
     continue;
   }

}

 pileupHist -> Fill(Tnpv);
  

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
PUTrueDistProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
PUTrueDistProducer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
PUTrueDistProducer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
PUTrueDistProducer::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
PUTrueDistProducer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
PUTrueDistProducer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
PUTrueDistProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(PUTrueDistProducer);
