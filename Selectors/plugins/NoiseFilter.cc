// -*- C++ -*-
//
// Package:    ExoDiBosonResonances/NoiseFilter
// Class:      NoiseFilter
// 
/**\class NoiseFilter NoiseFilter.cc ExoDiBosonResonances/NoiseFilter/plugins/NoiseFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ivan Shvetsov
//         Created:  Mon, 17 Nov 2014 13:35:20 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class NoiseFilter : public edm::EDFilter {
   public:
      explicit NoiseFilter(const edm::ParameterSet&);
      ~NoiseFilter();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() override;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
    
      edm::EDGetTokenT<edm::TriggerResults> NoiseFilterToken_;
      std::vector<std::string> vfilters;
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
NoiseFilter::NoiseFilter(const edm::ParameterSet& iConfig):
   NoiseFilterToken_(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("noiseFilter"))),
   vfilters(iConfig.getParameter<std::vector<std::string> >("filterNames"))
   
{
   //now do what ever initialization is needed

}


NoiseFilter::~NoiseFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool NoiseFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   edm::Handle<edm::TriggerResults> NoiseFiltersBits;
   iEvent.getByToken(NoiseFilterToken_, NoiseFiltersBits); 
   
   bool pass = true;

   edm::TriggerNames names = iEvent.triggerNames(*NoiseFiltersBits);

   for (unsigned int iTrig = 0; iTrig < NoiseFiltersBits -> size(); iTrig ++)
   {
      for (unsigned iName = 0; iName < vfilters.size(); iName ++)
      {
       if(vfilters[iName] == names.triggerName(iTrig) && (NoiseFiltersBits -> accept(iTrig) == false) ) pass = false;
      }
   }


   
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
NoiseFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NoiseFilter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
/*
void
NoiseFilter::beginRun(edm::Run const&, edm::EventSetup const&)
{ 
}
*/
 
// ------------ method called when ending the processing of a run  ------------
/*
void
NoiseFilter::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when starting to processes a luminosity block  ------------
/*
void
NoiseFilter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
NoiseFilter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/
 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NoiseFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(NoiseFilter);
