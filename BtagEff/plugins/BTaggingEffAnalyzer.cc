// -*- C++ -*-
//
// Package:    BTaggingEffAnalyzer
// Class:      BTaggingEffAnalyzer
// 
/**\class BTaggingEffAnalyzer BTaggingEffAnalyzer.cc Analysis/EDSHyFT/plugins/BTaggingEffAnalyzer.cc
 Description: [one line class summary]
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Dinko Ferencek
//         Created:  Thu Oct  4 20:25:54 CDT 2012
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TEfficiency.h"
typedef std::vector<pat::Jet> PatJetCollection;

//
// class declaration
//

class BTaggingEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit BTaggingEffAnalyzer(const edm::ParameterSet&);
      ~BTaggingEffAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      edm::EDGetTokenT<PatJetCollection> JetsToken;
      const edm::InputTag jetsTag;
      const std::string   discriminatorTag;
      const double  discriminatorValue;
      const int     ptNBins;
      const std::vector<double>  ptBinning;
      const int     etaNBins;
      const std::vector<double>  etaBinning;
      edm::Service<TFileService>  fs;
      TEfficiency  *BTaggingEff_b;
      TEfficiency  *BTaggingEff_c;
      TEfficiency  *BTaggingEff_udsg;
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
BTaggingEffAnalyzer::BTaggingEffAnalyzer(const edm::ParameterSet& iConfig) :
  JetsToken(consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("JetsTag"))),
  discriminatorTag(iConfig.getParameter<std::string>("DiscriminatorTag")),
  discriminatorValue(iConfig.getParameter<double>("DiscriminatorValue")),
  ptNBins(iConfig.getParameter<int>("PtNBins")),
  ptBinning(iConfig.getParameter<std::vector<double>>("ptBinning")),
  etaNBins(iConfig.getParameter<int>("EtaNBins")),
  etaBinning(iConfig.getParameter<std::vector<double>>("etaBinning"))

{
   //now do what ever initialization is needed
   BTaggingEff_b    = fs->make<TEfficiency>("BTaggingEff_b", ";p_{T} (GeV);#eta", ptNBins, &ptBinning[0], etaNBins, &etaBinning[0]);
   BTaggingEff_c    = fs->make<TEfficiency>("BTaggingEff_c", ";p_{T} (GeV);#eta", ptNBins, &ptBinning[0], etaNBins, &etaBinning[0]);
   BTaggingEff_udsg = fs->make<TEfficiency>("BTaggingEff_udsg", ";p_{T} (GeV);#eta", ptNBins, &ptBinning[0], etaNBins, &etaBinning[0]);
   BTaggingEff_b    -> SetStatisticOption(TEfficiency::kFCP);
   BTaggingEff_c    -> SetStatisticOption(TEfficiency::kFCP);
   BTaggingEff_udsg -> SetStatisticOption(TEfficiency::kFCP);

   
}


BTaggingEffAnalyzer::~BTaggingEffAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
BTaggingEffAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<PatJetCollection> jets;
  iEvent.getByToken(JetsToken,jets);

  // loop over jets
  for(PatJetCollection::const_iterator it = jets->begin(); it != jets->end(); ++it)
  {
    int partonFlavor = it->partonFlavour();
    bool passed;
    if( it->bDiscriminator(discriminatorTag.c_str()) >= discriminatorValue ) passed = true;
    else passed = false;

    if( abs(partonFlavor)==5 )
    {
      BTaggingEff_b->Fill(passed,it->pt(), it->eta());
    }
    else if( abs(partonFlavor)==4 )
    {
      BTaggingEff_c->Fill(passed,it->pt(), it->eta());
    }
    else
    {
      BTaggingEff_udsg->Fill(passed,it->pt(), it->eta());
    }
  }
}


// ------------ method called once each job just before starting event loop  ------------
void 
BTaggingEffAnalyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
BTaggingEffAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
BTaggingEffAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
BTaggingEffAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
BTaggingEffAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
BTaggingEffAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BTaggingEffAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(BTaggingEffAnalyzer);