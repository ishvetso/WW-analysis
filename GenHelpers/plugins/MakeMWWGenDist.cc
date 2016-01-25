// -*- C++ -*-
//
// Package:    ExoDiBosonResonances/MakeMWWGenDist
// Class:      MakeMWWGenDist
// 
/**\class MakeMWWGenDist MakeMWWGenDist.cc ExoDiBosonResonances/MakeMWWGenDist/plugins/MakeMWWGenDist.cc
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

class MakeMWWGenDist : public edm::EDAnalyzer {
   public:
      explicit MakeMWWGenDist(const edm::ParameterSet&);
      ~MakeMWWGenDist();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      TTree* outTree;
      edm::EDGetTokenT<LHEEventProduct> LHEEventProductToken;
      edm::EDGetTokenT<edm::View<reco::Candidate>> genParticlesToken;
       edm::EDGetTokenT<GenEventInfoProduct> genInfoToken;
      double m_WW;
      double weight8;
      double genWeight;
      double refXsec;
      int NWplus, NWminus;
      bool isSignal;
      std::map<std::string,double> aTGCWeights;
      std::map<std::string,double> *aTGCWeightsPointer = &aTGCWeights;
      //Decay Info (gen level)
     

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
MakeMWWGenDist::MakeMWWGenDist(const edm::ParameterSet& iConfig):
genParticlesToken(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("genSource"))),
genInfoToken(consumes<GenEventInfoProduct> (iConfig.getParameter<edm::InputTag>( "genInfo" ) )),
isSignal(iConfig.getParameter<bool>("isSignal"))
{
   //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree = fs->make<TTree>("Tree","Tree");
  outTree->Branch("m_WW",       &m_WW,        "m_WW/D"           );
  outTree->Branch("genWeight",       &genWeight,        "genWeight/D"           );
  outTree->Branch("NWplus",       &NWplus,        "NWplus/I"           );
  outTree->Branch("NWminus",       &NWminus,        "NWminus/I"           );
  if(isSignal){
    outTree->Branch("refXsec",       &refXsec,        "refXsec/D"           );
    outTree -> Branch("aTGCWeights", "std::map<std::string,double>", &aTGCWeightsPointer);
    LHEEventProductToken = mayConsume<LHEEventProduct> (iConfig.getParameter<edm::InputTag>( "LHEEventProductSource" ) );

  }


}


MakeMWWGenDist::~MakeMWWGenDist()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MakeMWWGenDist::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   Handle<edm::View<reco::Candidate> > genParticles;
   iEvent.getByToken(genParticlesToken, genParticles);

   Handle<LHEEventProduct> evtProduct;
   if(isSignal) iEvent.getByToken(LHEEventProductToken, evtProduct);

   edm::Handle <GenEventInfoProduct> genInfo;
   iEvent.getByToken(genInfoToken, genInfo); 

   ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4WPlus, p4WMinus;

   NWplus = 0;
   NWminus = 0;
   for (unsigned int iParticle = 0; iParticle < genParticles -> size(); iParticle ++){
     if (genParticles -> at(iParticle).pdgId() == 24 &&  genParticles -> at(iParticle).status() == 22) {
      p4WPlus = genParticles -> at(iParticle).p4();
      NWplus++;
    }
     else if (genParticles -> at(iParticle).pdgId() == -24 &&  genParticles -> at(iParticle).status() == 22) {
      p4WMinus = genParticles -> at(iParticle).p4();
      NWminus++;
    }
     else continue;
   }

  genWeight = (genInfo -> weight());
  m_WW = (p4WPlus + p4WMinus).mass();

  aTGCWeightsPointer -> clear();
  //aTGCWeightsPointer -> resize(8);

  if(isSignal){
   if( evtProduct->weights().size() ) {
      for ( size_t iwgt = 0; iwgt < evtProduct->weights().size(); ++iwgt ) {
        const LHEEventProduct::WGT& wgt = evtProduct->weights().at(iwgt);
        if( boost::algorithm::contains(wgt.id, "mg_reweight") ) aTGCWeights.insert(std::pair<std::string, double>(wgt.id, wgt.wgt));       
        
      }
    }
  
    refXsec = evtProduct -> originalXWGTUP();
  }  
   
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
MakeMWWGenDist::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MakeMWWGenDist::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
MakeMWWGenDist::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
MakeMWWGenDist::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
MakeMWWGenDist::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
MakeMWWGenDist::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MakeMWWGenDist::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeMWWGenDist);
