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
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  
#include "DataFormats/Math/interface/deltaR.h" 
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

namespace reco {
  typedef edm::Ptr<reco::GsfElectron> GsfElectronPtr;
}

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
      edm::EDGetTokenT<edm::View<reco::GsfElectron> > electronsToken;
      edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticlesToken;
      std::string channel;
      std::string variation;
      std::string type;
      

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
   if (channel != "mu" && channel != "el") std::cerr << "Invalid channel used, use el or mu" << std::endl; 
   type = iConfig.getParameter<std::string>("type");  
   if (type != "scale" && channel != "resolution") std::cerr << "Invalid type of uncertainty used, use scale or resolution" << std::endl; 
   leptonsToken = consumes<edm::View<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("leptonSource"));
   if (channel == "el") electronsToken = mayConsume<edm::View<reco::GsfElectron> >(iConfig.getParameter<edm::InputTag>("leptonSource"));
   if (type == "resolution") genParticlesToken = mayConsume<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("genSource"));
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

//lepton scale uncertainties
     if (type == "scale") {
      
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
    }

// lepton resolution uncertainties
    else if (type == "resolution") {
      edm::Handle<edm::View<reco::GenParticle> > genParticles;
      iEvent.getByToken(genParticlesToken, genParticles);
      bool isEB = false;
      double pt_gen = -1., pz_gen = -1. ;
      double pt = lepton.pt(), pz = lepton.pz();
      double pt_reco = lepton.pt(), pz_reco = lepton.pz();

      //do  matching 
      if (channel == "mu") {
     for (unsigned iGen = 0; iGen < genParticles -> size(); iGen ++){
        if ( deltaR(genParticles -> at(iGen), lepton) < 0.3 && lepton.pdgId() == genParticles -> at(iGen).pdgId() ) {
         pt_gen = genParticles -> at(iGen).pt();
         pz_gen = genParticles -> at(iGen).pz();
        }  
       }
     }
    // in electron channel need to define isEB, for that need to allocate reco::GsfElectron (or pat::Electron)
    if (channel == "el") {
      edm::Handle<edm::View<reco::GsfElectron>> electrons;
      iEvent.getByToken(electronsToken, electrons);

      auto elePtr = electrons -> ptrAt(0);
      reco::GsfElectronPtr eleGsfPtr(elePtr);
      const reco::CaloClusterPtr& seed = eleGsfPtr -> superCluster()->seed();
      isEB = ( seed->seed().subdetId() == EcalBarrel );
    }
     
   
     double ElectronResUncertainty = 0.1* (isEB ? 0.02 : 0.04 );//http://arxiv.org/pdf/1502.02701v2.pdf
     double MuonResUncertainty = 0.006; // https://twiki.cern.ch/twiki/bin/view/CMS/MuonReferenceResolution#General_recommendations_for_muon
     if (channel == "el" && variation == "up"){
          pt = pt + ElectronResUncertainty*pt;
          pz = pz + ElectronResUncertainty*pz;
      }
     else if (channel == "el" && variation == "down"){
          pt = pt - ElectronResUncertainty*pt;
          pz = pz - ElectronResUncertainty*pz;
      }
     else if (pt_gen > 0. && channel == "mu" && variation == "up") {
          pt = pt_gen + (1. + MuonResUncertainty)*(pt_reco - pt_gen);
          pz = pz_gen + (1. + MuonResUncertainty)*(pz_reco - pz_gen);
     }
     else if (pt_gen > 0. && channel == "mu" && variation == "down") {
          pt = pt_gen + (1. - MuonResUncertainty)*(pt_reco - pt_gen);
          pz = pz_gen + (1. - MuonResUncertainty)*(pz_reco - pz_gen);
     }
     else if (pt_gen ==  -1. && pz_gen == -1. && channel == "mu") {
      pt = pt_reco;
      pz = pz_reco;
    }
     else std::cerr << "Smth is wrong, either the variation is not correct (should be up or down) or channel is not correct (should be mu or el). Please check!" << std::endl;
     math::XYZTLorentzVector SmearedP4 = lepton.p4();
     SmearedP4.SetPx(pt * cos(lepton.phi()));
     SmearedP4.SetPy(pt * sin(lepton.phi()));
     SmearedP4.SetPz(pz);
     SmearedP4.SetE(sqrt(pt*pt + pz*pz + (lepton.mass()) * (lepton.mass()) ) );
     lepton.setP4(SmearedP4);

   }
    else std::cerr <<  "Invalid type of uncertainty used, use scale or resolution" << std::endl;
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
