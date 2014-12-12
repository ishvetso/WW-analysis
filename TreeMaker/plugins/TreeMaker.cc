// system include files
#include <iostream>
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"


#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TTree.h"
#include "TFile.h"

#include "DecayChannel.h"
#include "DecayClass.h"
#include "Particle.h"


//
// class declaration
//

class TreeMaker : public edm::EDAnalyzer {
public:
  explicit TreeMaker(const edm::ParameterSet&);
  ~TreeMaker();
  //static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  TTree* outTree_;

  int nVtx;
  Particle Wboson_lep, Wboson_had, METCand, Electron, Muon, Lepton;
  
  //Decay Info (gen level)
  DecayClass WDecayClass;
  
  int N_had_W, N_lep_W;
  int N_had_Wgen, N_lep_Wgen;

  double tau1, tau2, tau3, tau21;

  
  // Electron ID 
  double etaSC_ele;
  double dEtaIn_ele;
  double dPhiIn_ele;
  double hcalOverEcal;
  double full5x5_sigma;
  double ooEmooP;
  double d0;
  double dz;
  double relIso;
  int missingHits;
  int passConVeto;
  
  //Muon IDs
  double pt_muon, eta_muon, phi_muon;
  
  double deltaR_LepWJet, deltaPhi_LepMet, deltaPhi_WJetMet;

  /// Parameters to steer the treeDumper
  std::string hadronicVSrc_, leptonicVSrc_, genSrc_;
  std::string  metSrc_;
};

//
// constructors and destructor
//
TreeMaker::TreeMaker(const edm::ParameterSet& iConfig)

{
  // Sources
  hadronicVSrc_ = iConfig.getParameter<std::string>("hadronicVSrc");
  leptonicVSrc_ = iConfig.getParameter<std::string>("leptonicVSrc");
  genSrc_ = iConfig.getParameter<std::string>("genSrc");
   metSrc_= iConfig.getParameter<std::string>("metSrc");

  
  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree_ = fs->make<TTree>("Basic Info","Basic Info");

  //W observables
  outTree_->Branch("pt_W_lep"        ,&Wboson_lep.pt  ,"pt_W_lep/D"         );
  outTree_->Branch("pt_W_had"        ,&Wboson_had.pt  ,"pt_W_had/D"         );
  
  outTree_->Branch("eta_W_lep"       ,&Wboson_lep.eta ,"eta_W_lep/D"         );
  outTree_->Branch("eta_W_had"       ,&Wboson_had.eta ,"eta_W_had/D"         );
  
  outTree_->Branch("phi_W_lep"       ,&Wboson_lep.phi, "phi_W_lep/D"         );
  outTree_->Branch("phi_W_had"       ,&Wboson_had.phi ,"phi_W_had/D"         );
  
  outTree_->Branch("mass_W_lep"      ,&Wboson_lep.mass,"mass_W_lep/D"         );
  outTree_->Branch("mass_W_had"      ,&Wboson_had.mass,"mass_W_had/D"         );
  
  outTree_->Branch("mt_W_lep"        ,&Wboson_lep.mt, "mt_W_lep/D"         );
  outTree_->Branch("mt_W_had"        ,&Wboson_had.mt,  "mt_W_had/D"         );
  
  outTree_->Branch("N_had_W"          ,&N_had_W       ,"N_had_W/I"         );
  outTree_->Branch("N_lep_W"          ,&N_lep_W       ,"N_lep_W/I"         );
  
  outTree_->Branch("WDecayClass"          ,&WDecayClass       ,"WDecayClass/I"         );
  
  outTree_->Branch("N_had_W_gen"          ,&N_had_Wgen       ,"N_had_W_gen/I"         );
  outTree_->Branch("N_lep_W_gen"          ,&N_lep_Wgen      ,"N_lep_W_gen/I"         );
  
  outTree_->Branch("tau1"            ,&tau1           ,"tau1/D"           );
  outTree_->Branch("tau2"            ,&tau2           ,"tau2/D"           );
  outTree_->Branch("tau3"            ,&tau3           ,"tau3/D"           );
  outTree_->Branch("tau21"           ,&tau21          ,"tau21/D"          );


    
  outTree_->Branch("MET"        ,&METCand.pt, "MET/D"         );
  outTree_->Branch("MET_eta"        ,&METCand.eta, "MET_eta/D"         );
  outTree_->Branch("MET_phi"        ,&METCand.phi, "MET_phi/D"         );
  outTree_->Branch("MET_mass"        ,&METCand.mass, "MET_mass/D"         );
  outTree_->Branch("MET_mt"        ,&METCand.mt, "MET_mt/D"         );

  
  /// Electron ID quantities
  outTree_->Branch("pt_ele"    ,&Electron.pt     ,"pt_ele/D"     );
  outTree_->Branch("eta_ele"    ,&Electron.eta     ,"eta_ele/D"     );
  outTree_->Branch("phi_ele"    ,&Electron.phi     ,"phi_ele/D"     );
 
  outTree_->Branch("etaSC_ele"          ,&etaSC_ele         ,"etaSC_ele/D"         );
  outTree_->Branch("dEtaIn_ele"         ,&dEtaIn_ele        ,"dEtaIn_ele/D"        );
  outTree_->Branch("dPhiIn_ele"         ,&dPhiIn_ele        ,"dPhiIn_ele/D"        );
  outTree_->Branch("hcalOverEcal"         ,&hcalOverEcal ,"hcalOverEcal/D"        );
  outTree_->Branch("full5x5_sigma"  ,&full5x5_sigma ,"full5x5_sigma/D" );
  outTree_->Branch("ooEmooP"        ,&ooEmooP       ,"ooEmooP/D"       );
  outTree_->Branch("d0"             ,&d0            ,"d0/D"            );
  outTree_->Branch("dz"             ,&dz            ,"dz/D"            );
  
  outTree_->Branch("relIso"         ,&relIso        ,"relIso/D"        );
  outTree_->Branch("missingHits"    ,&missingHits   ,"missingHits/I"   );
  outTree_->Branch("passConVeto"    ,&passConVeto   ,"passConVeto/I"   );
 
 
   /// Electron ID quantities
  outTree_->Branch("pt_mu",  &Muon.pt, "pt_mu/D"          );
  outTree_->Branch("eta_mu", &Muon.eta , "eta_mu/D"          );
  outTree_->Branch("phi_mu", &Muon.phi, "phi_mu/D"          );
 // outTree_->Branch("ptel2"           ,&ptel2          ,"ptel2/D"          );
  //outTree_->Branch("etaSC1"          ,&etaSC1         ,"etaSC1/D"         );
  //outTree_->Branch("etaSC2"          ,&etaSC2         ,"etaSC2/D"         );
  //outTree_->Branch("dEtaIn1"         ,&dEtaIn1        ,"dEtaIn1/D"        );
 // outTree_->Branch("dEtaIn2"         ,&dEtaIn2        ,"dEtaIn2/D"        );
  //outTree_->Branch("dPhiIn1"         ,&dPhiIn1        ,"dPhiIn1/D"        );
  //outTree_->Branch("dPhiIn2"         ,&dPhiIn2        ,"dPhiIn2/D"        );
 /* outTree_->Branch("hOverE1"         ,&hOverE1        ,"hOverE1/D"        );
  outTree_->Branch("hOverE2"         ,&hOverE2        ,"hOverE2/D"        );
  outTree_->Branch("full5x5_sigma1"  ,&full5x5_sigma1 ,"full5x5_sigma1/D" );
  outTree_->Branch("full5x5_sigma2"  ,&full5x5_sigma2 ,"full5x5_sigma2/D" );
  outTree_->Branch("ooEmooP1"        ,&ooEmooP1       ,"ooEmooP1/D"       );
  outTree_->Branch("ooEmooP2"        ,&ooEmooP2       ,"ooEmooP2/D"       );
  outTree_->Branch("d01"             ,&d01            ,"d01/D"            );
  //outTree_->Branch("d02"             ,&d02            ,"d02/D"            );
  outTree_->Branch("dz1"             ,&dz1            ,"dz1/D"            );
  //outTree_->Branch("dz2"             ,&dz2            ,"dz2/D"            );
  outTree_->Branch("relIso1"         ,&relIso1        ,"relIso1/D"        );
  outTree_->Branch("relIso2"         ,&relIso2        ,"relIso2/D"        );
  outTree_->Branch("missingHits1"    ,&missingHits1   ,"missingHits1/I"   );
  outTree_->Branch("missingHits2"    ,&missingHits2   ,"missingHits2/I"   );
  outTree_->Branch("passConVeto1"    ,&passConVeto1   ,"passConVeto1/I"   );
  outTree_->Branch("passConVeto2"    ,&passConVeto2   ,"passConVeto2/I"   );*/
 
  /// Generic kinematic quantities
  outTree_->Branch("pt_lep"          ,&Lepton.pt         ,"pt_lep/D"         );
  outTree_->Branch("eta_lep"          ,&Lepton.eta         ,"eta_lep/D"         );
  outTree_->Branch("phi_lep"          ,&Lepton.phi         ,"phi_lep/D"         );
  
  /// Other quantities
  outTree_->Branch("deltaR_LepWJet"    ,&deltaR_LepWJet   ,"deltaR_LepWJet/D"   );
  outTree_->Branch("deltaPhi_LepMet"    ,&deltaPhi_LepMet   ,"deltaPhi_LepMet/D"   );
  outTree_->Branch("deltaPhi_WJetMet"    ,&deltaPhi_WJetMet   ,"deltaPhi_WJetMet/D"   );
}


TreeMaker::~TreeMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   //Hadronic Ws
   edm::Handle<edm::View<pat::Jet> > hadronicVs;
   iEvent.getByLabel(hadronicVSrc_.c_str(), hadronicVs);
   
   //Leptonic Ws
   edm::Handle<edm::View<reco::Candidate> > leptonicVs;
   iEvent.getByLabel(leptonicVSrc_.c_str(), leptonicVs);
   
   //GenParticles
   edm::Handle<edm::View<reco::Candidate> > genParticles;
   iEvent.getByLabel(genSrc_.c_str(), genParticles); 
   
   //Vertices 
   edm::Handle<edm::View<reco::Vertex> > vertices;
   iEvent.getByLabel("offlineSlimmedPrimaryVertices", vertices);
   
   //MET
   edm::Handle<edm::View<pat::MET> > metHandle;
   iEvent.getByLabel(metSrc_.c_str(), metHandle);

   nVtx = vertices->size();

   //  Defining decay channel on the gen level
   N_had_Wgen  = 0, N_lep_Wgen = 0 ;

   DefineDecayChannel(genParticles, N_lep_Wgen , N_had_Wgen );
   
   if (N_lep_Wgen == 0 && N_had_Wgen == 2  ) WDecayClass = Hadronic;
   else if (N_lep_Wgen == 1 && N_had_Wgen == 1 ) WDecayClass = Semileptonic;
   else if (N_lep_Wgen == 2 && N_had_Wgen == 0 ) WDecayClass = Leptonic;
   else WDecayClass = UnDefined;

   N_lep_W = leptonicVs -> size();
   N_had_W = hadronicVs -> size();
   

  
   const pat::Jet& hadronicV = hadronicVs->at(0);
   const reco::Candidate& leptonicV = leptonicVs->at(0);
   const pat::MET& metCand = metHandle->at(0);
   
   Wboson_lep.pt = leptonicV.pt();
   Wboson_lep.eta = leptonicV.eta();
   Wboson_lep.phi = leptonicV.phi();
   Wboson_lep.mass = leptonicV.mass();
   Wboson_lep.mt = leptonicV.mt();
   
   Wboson_had.pt = hadronicV.pt();
   Wboson_had.eta = hadronicV.eta();
   Wboson_had.phi = hadronicV.phi();
   Wboson_had.mass = hadronicV.userFloat("ak8PFJetsCHSPrunedLinks");
   Wboson_had.mt = hadronicV.mt();

   tau1 = hadronicV.userFloat("tau1");
   tau2 = hadronicV.userFloat("tau2");
   tau3 = hadronicV.userFloat("tau3");
   tau21 = hadronicV.userFloat("tau21");
   
   METCand.pt = metCand.pt();
   METCand.eta = metCand.eta();
   METCand.phi = metCand.phi();
   METCand.mass = metCand.mass();
   METCand.mt = metCand.mt();

   //Running over leptons
   
   int ElectronDaughterIndex = -1, MuonDaughterIndex = -1, LeptonDaughterIndex = -1;
   
   for (unsigned int iDaughter = 0; iDaughter < leptonicV.numberOfDaughters(); iDaughter++)
   {
     if ( leptonicV.daughter(iDaughter) -> isElectron()) { ElectronDaughterIndex = iDaughter; LeptonDaughterIndex = iDaughter;};
     if ( leptonicV.daughter(iDaughter) -> isMuon()) {MuonDaughterIndex = iDaughter; LeptonDaughterIndex = iDaughter;};
   }
     
   if( leptonicV.daughter(0)->isElectron() || leptonicV.daughter(1)->isElectron()) {
      
      const pat::Electron *el = (pat::Electron*)leptonicV.daughter(ElectronDaughterIndex);
    
      Electron.pt  = el->pt() ;
      Electron.eta = el -> eta();
      Electron.phi = el -> phi();
    
      etaSC_ele  = el -> superCluster()->eta();
      dEtaIn_ele = el -> deltaEtaSuperClusterTrackAtVtx();
      dPhiIn_ele = el -> deltaPhiSuperClusterTrackAtVtx();
      hcalOverEcal = el->hcalOverEcal();
      
      full5x5_sigma = el->full5x5_sigmaIetaIeta();
      
      if( el->ecalEnergy() == 0 ){
	printf("Electron energy is zero!\n");
	ooEmooP = 1e30;
      }else if( !std::isfinite(el->ecalEnergy())){
	printf("Electron energy is not finite!\n");
	ooEmooP = 1e30;
      }else{
	ooEmooP = fabs(1.0/el->ecalEnergy() - el->eSuperClusterOverP()/el->ecalEnergy() );
      }

      d0 = (-1)*el->gsfTrack()->dxy(math::XYZPoint(0.,0.,0.)); //Distance wrt origin is not correct  
      //Implementation of primary vertex needed! 
      dz = el->gsfTrack()->dz(math::XYZPoint(0.,0.,0.));
     
      reco::GsfElectron::PflowIsolationVariables pfIso = el->pfIsolationVariables();
      double absiso = pfIso.sumChargedHadronPt + std::max(0.0, pfIso.sumNeutralHadronEt + pfIso.sumPhotonEt - 0.5*pfIso.sumPUPt );
      relIso = absiso/el->pt();
      
      missingHits = el -> gsfTrack()->hitPattern().numberOfLostHits(reco::HitPattern::MISSING_INNER_HITS);
      passConVeto = el->passConversionVeto();
      
     
    }
    
      else {
      Electron.pt = -99.;
      Electron.eta = - 99.;
      Electron.phi = - 99.;
      
      etaSC_ele = -99.;
      dEtaIn_ele = -99.;
      dPhiIn_ele = -99.;
      hcalOverEcal = -99. ;
      full5x5_sigma = -99.;
      ooEmooP = -99.;
      d0 = -99.;
      dz = -99.;
      relIso = -99.;
      missingHits = -99;
      passConVeto = -99;
    }
    
      // Running over muons
     if( leptonicV.daughter(0)->isMuon() || leptonicV.daughter(1)->isMuon()) {
       
     const pat::Muon *muon = (pat::Muon*)leptonicV.daughter(MuonDaughterIndex);
      Muon.pt   = muon -> pt() ;
      Muon.eta   = muon -> eta();
      Muon.phi  = muon -> phi();
  
      }
   
    else {
       Muon.pt = -99.;
       Muon.eta  = -99.;
       Muon.phi = -99.0;
     }
  
   
   
   // Kinematics of leptons and jets
   Lepton.pt = (leptonicV.daughter(LeptonDaughterIndex))->pt();
   Lepton.eta = (leptonicV.daughter(LeptonDaughterIndex))->eta();
   Lepton.phi = (leptonicV.daughter(LeptonDaughterIndex))->phi();
   

   deltaR_LepWJet = deltaR(Lepton.eta,Lepton.phi,Wboson_had.eta,Wboson_had.phi); 
   deltaPhi_LepMet = deltaPhi(Lepton.phi, METCand.phi);
   deltaPhi_WJetMet = deltaPhi(Wboson_had.phi, METCand.phi);

   outTree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
TreeMaker::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TreeMaker::endJob() {
  std::cout << "TreeMaker endJob()..." << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TreeMaker);
