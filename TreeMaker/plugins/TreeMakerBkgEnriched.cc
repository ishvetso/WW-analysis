// system include files
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm> 
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
#include <DataFormats/MuonReco/interface/Muon.h>


#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TLorentzVector.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "DecayChannel.h"
#include "DecayClass.h"
#include "Particle.h"


//
// class declaration
//

class TreeMakerBkgEnriched : public edm::EDAnalyzer {
public:
  explicit TreeMakerBkgEnriched(const edm::ParameterSet&);
  ~TreeMakerBkgEnriched();
  //static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  // ----------member data ---------------------------
  TTree* outTree_;

  //event info
  int nevent, run, lumi;
  
  //number of primary vertices
  int nPV;
  
  Particle Wboson_lep, Wboson_had, METCand, Electron, Muon, Lepton;
  double m_pruned;
  
  //Decay Info (gen level)
  DecayClass WDecayClass;
  
  int N_had_W, N_lep_W;
  int N_had_Wgen, N_lep_Wgen;
  
  int nLooseEle, nLooseMu, nLep;

  double tau1, tau2, tau3, tau21;
  
  double deltaR_LepWJet, deltaPhi_LepMet, deltaPhi_WJetMet, deltaPhi_WJetWlep;
  
  //Jets
  int NAK8jet, njets, nbtag;
  double jet_pt, jet_eta, jet_phi, jet_mass_pruned, jet_mass_softdrop, jet_tau2tau1;
  
  //AK4 jets
  double jet2_pt, jet2_btag, jet3_pt, jet3_btag;
  
  //m_lvj
  double m_lvj;
  
  //Defining Tokens
  edm::EDGetTokenT<edm::View<pat::MET> > metToken_;
  edm::EDGetTokenT<edm::View<pat::Jet>> hadronicVToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> leptonicVToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> genParticlesToken_;
  edm::EDGetTokenT<edm::View<pat::Jet>> fatJetsToken_;
  edm::EDGetTokenT<edm::View<pat::Jet>> AK4JetsToken_;
  edm::EDGetTokenT<edm::View<reco::Vertex> > vertexToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> looseMuToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> looseEleToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> leptonsToken_;
  bool isMC;

};

//
// constructors and destructor
//
TreeMakerBkgEnriched::TreeMakerBkgEnriched(const edm::ParameterSet& iConfig):
  metToken_(consumes<edm::View<pat::MET>>(iConfig.getParameter<edm::InputTag>("metSrc"))),
  hadronicVToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("hadronicVSrc"))),
  leptonicVToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonicVSrc"))),
  genParticlesToken_(mayConsume<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("genSrc"))),
  fatJetsToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("fatJetSrc"))),
  AK4JetsToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("AK4JetSrc"))),
  vertexToken_(consumes<edm::View<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
  looseMuToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("looseMuSrc"))),
  looseEleToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("looseEleSrc"))),
  leptonsToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonSrc"))),
  isMC(iConfig.getParameter<bool>("isMC"))
{
  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree_ = fs->make<TTree>("BasicTree","BasicTree");

  //event info
  outTree_->Branch("event",	      &nevent,    	  "event/I"           );
  outTree_->Branch("lumi", 	      &lumi,   		  "lumi/I"  		);
  outTree_->Branch("run",	      &run,		  "run/I"  	       );
  
  //number of primary vertices
  outTree_->Branch("nPV",	      &nPV,		  "nPV/I"  	       );
  
  //number of loose leptons
  outTree_->Branch("nLooseEle",      &nLooseEle, 	  "nLooseEle/I"       );
  outTree_->Branch("nLooseMu",      &nLooseMu, 	  "nLooseMu/I"       );
  
  // number of leptons for particular mu/ele channel (should be tight lepton)
  outTree_->Branch("nLep",            &nLep, 	      "nLep/I"       );
  
  //leptons (tight, ele or mu depends on the channel)
  outTree_->Branch("l_pt",	      &Lepton.pt,      "l_pt/D"         	);
  outTree_->Branch("l_eta",	      &Lepton.eta,     "l_eta/D"        	);
  outTree_->Branch("l_phi",	      &Lepton.phi,     "l_phi/D"        	);
  
  //W observables
  outTree_->Branch("W_pt",	      &Wboson_lep.pt,     "W_pt/D"         );
  outTree_->Branch("pt_W_had",	      &Wboson_had.pt,     "pt_W_had/D"         );
  
  outTree_->Branch("W_eta",	      &Wboson_lep.eta,    "W_eta/D"        );
  outTree_->Branch("eta_W_had",	      &Wboson_had.eta,    "eta_W_had/D"        );
  
  outTree_->Branch("W_phi",	      &Wboson_lep.phi,    "W_phi/D"        );
  outTree_->Branch("phi_W_had",	      &Wboson_had.phi,    "phi_W_had/D"        );
  
  outTree_->Branch("mass_W_lep",      &Wboson_lep.mass,   "mass_W_lep/D"       );
  outTree_->Branch("mass_W_had",      &Wboson_had.mass,   "mass_W_had/D"       );
  outTree_->Branch("mass_W_pruned",   &m_pruned,          "mass_W_pruned/D"    );
  
  outTree_->Branch("mt_W_lep",	      &Wboson_lep.mt,     "mt_W_lep/D"         );
  outTree_->Branch("mt_W_had",	      &Wboson_had.mt,  	  "mt_W_had/D"         );
  
  outTree_->Branch("charge_W_lep",    &Wboson_lep.charge, "charge_W_lep/D"     );
    
  outTree_->Branch("N_had_W",	      &N_had_W, 	  "N_had_W/I"          );
  outTree_->Branch("N_lep_W",	      &N_lep_W,		  "N_lep_W/I"          );
  
  outTree_->Branch("WDecayClass",     &WDecayClass,	  "WDecayClass/I"      );
  
  outTree_->Branch("N_had_W_gen",     &N_had_Wgen,	  "N_had_W_gen/I"      );
  outTree_->Branch("N_lep_W_gen",     &N_lep_Wgen, 	  "N_lep_W_gen/I"      );
  
  outTree_->Branch("tau1",	      &tau1,		  "tau1/D"             );
  outTree_->Branch("tau2",	      &tau2,		  "tau2/D"             );
  outTree_->Branch("tau3",	      &tau3,		  "tau3/D"             );
  outTree_->Branch("tau21",	      &tau21,		  "tau21/D"            );


  //MET observables  
  outTree_->Branch("pfMET", 	      &METCand.pt, 	  "pfMET/D"              );
  outTree_->Branch("pfMETPhi",	      &METCand.phi, 	  "pfMETPhi/D"          );
  outTree_->Branch("pfMETMass",	      &METCand.mass, 	  "pfMETMass/D"         );
  outTree_->Branch("pfMETMt",	      &METCand.mt, 	  "pfMETMt/D"           );

  /// Other observables
  outTree_->Branch("deltaR_LepWJet",  &deltaR_LepWJet,	  "deltaR_LepWJet/D"   );
  outTree_->Branch("deltaPhi_LepMet", &deltaPhi_LepMet,	  "deltaPhi_LepMet/D"  );
  outTree_->Branch("deltaPhi_WJetMet",&deltaPhi_WJetMet,  "deltaPhi_WJetMet/D" );
  outTree_->Branch("deltaPhi_WJetWlep",&deltaPhi_WJetWlep,  "deltaPhi_WJetWlep/D" );
  //Jet observables
  outTree_->Branch("NAK8jet",            &NAK8jet,              "NAK8jet/I"   );
  outTree_->Branch("jet_pt",  	      &jet_pt,	  	  "jet_pt/D"   );
  outTree_->Branch("jet_eta",  	      &jet_eta,	  	  "jet_eta/D"   );
  outTree_->Branch("jet_phi",  	      &jet_phi,	  	  "jet_phi/D"   );
  outTree_->Branch("jet_mass_pruned", &jet_mass_pruned,	  "jet_mass_pruned/D"   );
  outTree_->Branch("jet_mass_softdrop",&jet_mass_softdrop,"jet_mass_softdrop/D"   );
  outTree_->Branch("jet_tau2tau1",    &jet_tau2tau1,	  "jet_tau2tau1/D"   );
  
  
  outTree_->Branch("njets",  	      &njets,	          "njets/I"   );
  outTree_->Branch("nbtag",  	      &nbtag,	          "nbtag/I"   );
  
  outTree_->Branch("jet2_pt",  	      &jet2_pt,	          "jet2_pt/D"   );
  outTree_->Branch("jet2_btag",       &jet2_btag,         "jet2_btag/D"   );
  outTree_->Branch("jet3_pt",  	      &jet3_pt,	          "jet3_pt/D"   );
  outTree_->Branch("jet3_btag",	      &jet3_btag,         "jet3_btag/D"   );
  
  outTree_->Branch("m_lvj",	      &m_lvj,         "m_lvj/D"   );
}


TreeMakerBkgEnriched::~TreeMakerBkgEnriched()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TreeMakerBkgEnriched::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   nevent = iEvent.eventAuxiliary().event();
   run    = iEvent.eventAuxiliary().run();
   lumi   = iEvent.eventAuxiliary().luminosityBlock();
   
   //Hadronic Ws
   edm::Handle<edm::View<pat::Jet> > hadronicVs;
   iEvent.getByToken(hadronicVToken_, hadronicVs);
   
   //Leptonic Ws
   edm::Handle<edm::View<reco::Candidate> > leptonicVs;
   iEvent.getByToken(leptonicVToken_, leptonicVs);
   
   //GenParticles
   edm::Handle<edm::View<reco::Candidate> > genParticles;
   if(isMC)iEvent.getByToken(genParticlesToken_, genParticles); 
   
   //Vertices 
   edm::Handle<edm::View<reco::Vertex> > vertices;
   iEvent.getByToken(vertexToken_, vertices);
   
   //Jets
   edm::Handle<edm::View<pat::Jet> > jets; 
   iEvent.getByToken(fatJetsToken_, jets);
   
   //JAK4 ets (for Btag veto )
   edm::Handle<edm::View<pat::Jet> > AK4Jets;
   iEvent.getByToken(AK4JetsToken_, AK4Jets);
   
   //MET
   edm::Handle<edm::View<pat::MET> > metHandle;
   iEvent.getByToken(metToken_, metHandle);
   
   //loose electrons
   edm::Handle<edm::View<reco::Candidate> > looseElectrons;
   iEvent.getByToken(looseEleToken_, looseElectrons);
   
   //loose muons
   edm::Handle<edm::View<reco::Candidate> > looseMuons;
   iEvent.getByToken(looseMuToken_, looseMuons); 
   
   //leptons (tight)
   edm::Handle<edm::View<reco::Candidate> > leptons;
   iEvent.getByToken(leptonsToken_, leptons); 

   nPV = vertices->size();
      
   //  Defining decay channel on the gen level
   N_had_Wgen  = 0, N_lep_Wgen = 0 ;

   if (isMC) DefineDecayChannel(genParticles, N_lep_Wgen , N_had_Wgen );
   
   if (N_lep_Wgen == 0 && N_had_Wgen == 2  ) WDecayClass = Hadronic;
   else if (N_lep_Wgen == 1 && N_had_Wgen == 1 ) WDecayClass = Semileptonic;
   else if (N_lep_Wgen == 2 && N_had_Wgen == 0 ) WDecayClass = Leptonic;
   else WDecayClass = UnDefined;

   N_lep_W = leptonicVs -> size();
   N_had_W = hadronicVs -> size();   
   
   //loose leptons
   nLooseEle = looseElectrons -> size();
   nLooseMu = looseMuons -> size();
   
   nLep = leptons -> size();
   
   //tight leptons
   if ( ( leptons -> size() ) > 0 )
   {
     Lepton.pt = (leptons -> at(0)).pt();
     Lepton.eta = (leptons -> at(0)).eta();
     Lepton.phi = (leptons -> at(0)).phi();
   }
   
   else 
   {
     Lepton.pt = -99.;
     Lepton.eta = -99.;
     Lepton.phi = -99.;
   }
  

   //leptonically decaying W
   if (leptonicVs -> size() > 0)
   {
      const reco::Candidate  & leptonicV = leptonicVs->at(0);   
      Wboson_lep.pt = leptonicV.pt();
      Wboson_lep.eta = leptonicV.eta();
      Wboson_lep.phi = leptonicV.phi();
      Wboson_lep.mass = leptonicV.mass();
      Wboson_lep.mt = leptonicV.mt();
      Wboson_lep.charge = leptonicV.charge();
  }
  
  else 
   {
      Wboson_lep.pt = -99.;
      Wboson_lep.eta = -99.;
      Wboson_lep.phi = -99.;
      Wboson_lep.mass = -99.;
      Wboson_lep.mt = -99.;
      Wboson_lep.charge = -99.;
  }
  
  //hadronically W 
  if (hadronicVs -> size() > 0)
  {
    const pat::Jet& hadronicV = hadronicVs->at(0);
      
    Wboson_had.pt = hadronicV.pt();
    Wboson_had.eta = hadronicV.eta();
    Wboson_had.phi = hadronicV.phi();
    Wboson_had.mass = hadronicV.mass();
    Wboson_had.mt = hadronicV.mt();
    m_pruned = hadronicV.userFloat("ak8PFJetsCHSPrunedMass");   

    tau1 = hadronicV.userFloat("NjettinessAK8:tau1");
    tau2 = hadronicV.userFloat("NjettinessAK8:tau2");
    tau3 = hadronicV.userFloat("NjettinessAK8:tau3");
    tau21 = tau2/tau1;
  }
   
   else 
   {
     Wboson_had.pt = -99.;
     Wboson_had.eta = -99.;
     Wboson_had.phi = -99.;
     Wboson_had.mass = -99.;
     Wboson_had.mt = -99.;
     m_pruned = -99.;   

     tau1 = -99.;
     tau2 = -99.;
     tau3 = -99.;
     tau21 = -99.;     
   }

    //MET quantities   
   if (metHandle->size() > 0)
   {
      const pat::MET& metCand = metHandle->at(0);
      
      METCand.pt = metCand.pt();
      METCand.phi = metCand.phi();
      METCand.mass = metCand.mass();
      METCand.mt = metCand.mt();
   }
   
    else
   {
      METCand.pt = -99.;
      METCand.phi = -99.;
      METCand.mass = -99.;
      METCand.mt = -99.;
   }
   
    if (hadronicVs -> size() > 0 && leptonicVs -> size() > 0)
    {
      deltaR_LepWJet = deltaR(Lepton.eta,Lepton.phi,Wboson_had.eta,Wboson_had.phi); 
      deltaPhi_LepMet = deltaPhi(Lepton.phi, METCand.phi);
      deltaPhi_WJetMet = deltaPhi(Wboson_had.phi, METCand.phi);
      deltaPhi_WJetWlep = deltaPhi(Wboson_had.phi, Wboson_lep.phi);
    }
    else 
    {
      deltaR_LepWJet = -99.; 
      deltaPhi_LepMet = -99.;
      deltaPhi_WJetMet = -99.;
      deltaPhi_WJetWlep = -99.;
    }
   
  
  NAK8jet = jets -> size();

   
   if (jets -> size() > 0)
  {
    jet_pt = (jets -> at(0)).pt();
    jet_eta = (jets -> at(0)).eta();
    jet_phi = (jets -> at(0)).phi();
    jet_mass_pruned = (jets -> at(0)).userFloat("ak8PFJetsCHSPrunedMass");
    jet_mass_softdrop = (jets -> at(0)).userFloat("ak8PFJetsCHSSoftDropMass");
    jet_tau2tau1 = ((jets -> at(0)).userFloat("NjettinessAK8:tau2"))/((jets -> at(0)).userFloat("NjettinessAK8:tau1"));
  }
  
  else 
  {
    jet_pt = -99.;
    jet_eta = -99.;
    jet_phi = -99.;
    jet_mass_pruned = -99.;
    jet_mass_softdrop = -99.;
    jet_tau2tau1 = -99.;
  }
  
  
   
  //Loop over the collection of the AK4 jets which contain b-tagging information (to veto b-jets)
  njets = AK4Jets -> size(); 
  nbtag = 0;
  
  for (unsigned int iBtag = 0; iBtag < AK4Jets -> size(); iBtag ++)
  {
    //WP for 8 TeV and preliminary. Should be updated at some point
    if(((AK4Jets -> at(iBtag)).bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags")) > 0.890) nbtag ++;
  }
 
  if (AK4Jets -> size() > 0)
  {
    jet2_pt = (AK4Jets -> at(0)).pt();
    jet2_btag = (AK4Jets -> at(0)).bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
  }
  
  else 
  {
    jet2_pt = -99.;
    jet2_btag = -99.;
  }
  
  if (AK4Jets -> size() > 1)
  {
    jet3_pt = (AK4Jets -> at(1)).pt();
    jet3_btag = (AK4Jets -> at(1)).bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
  } 
  else 
  {
    jet3_pt = -99.;
    jet3_btag = -99.;
  }
  
  //diboson mass
   TLorentzVector hadronicVp4, leptonicVp4, lvj_p4;
   hadronicVp4.SetPtEtaPhiM(Wboson_had.pt,Wboson_had.eta,Wboson_had.phi,Wboson_had.mass);
   leptonicVp4.SetPtEtaPhiM(Wboson_lep.pt,Wboson_lep.eta,Wboson_lep.phi,Wboson_lep.mass);
   lvj_p4 = hadronicVp4 + leptonicVp4;
   if (leptonicVs -> size() > 0 && hadronicVs -> size() > 0)   m_lvj = lvj_p4.M();
   else m_lvj = -99.;


 // uncomment the line used in the synchronization exercise!
 if (deltaR_LepWJet > (TMath::Pi()/2.0) && fabs(deltaPhi_WJetMet) > 2. && fabs(deltaPhi_WJetWlep) > 2. && nbtag >= 1 && Wboson_lep.pt > 200. && jet_mass_pruned <= 40. && jet_mass_pruned >= 130. && jet_tau2tau1 < 0.5) outTree_->Fill();
   //outTree_->Fill();

}


// ------------ method called once each job just before starting event loop  ------------
void 
TreeMakerBkgEnriched::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
TreeMakerBkgEnriched::endJob() {
  std::cout << "TreeMakerBkgEnriched endJob()..." << std::endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TreeMakerBkgEnriched);
