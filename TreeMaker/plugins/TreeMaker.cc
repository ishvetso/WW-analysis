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
#include "DataFormats/Math/interface/deltaR.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

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
  
  /// Parameters to steer the treeDumper
  std::string hadronicVSrc_, leptonicVSrc_, genSrc_, metSrc_, jetsSrc_, jets_btag_veto_Src_, vertexSrc_, looseEleSrc_, looseMuSrc_, leptonSrc_;
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
  jetsSrc_ = iConfig.getParameter<std::string>("jetSrc");
  jets_btag_veto_Src_ = iConfig.getParameter<std::string>("jets_btag_veto_Src");
  vertexSrc_ = iConfig.getParameter<std::string>("vertex_Src");
  looseEleSrc_ = iConfig.getParameter<std::string>("looseEleSrc");
  looseMuSrc_ = iConfig.getParameter<std::string>("looseMuSrc");
  leptonSrc_ = iConfig.getParameter<std::string>("leptonSrc");

  
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

   nevent = iEvent.eventAuxiliary().event();
   run    = iEvent.eventAuxiliary().run();
   lumi   = iEvent.eventAuxiliary().luminosityBlock();
   
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
   iEvent.getByLabel(vertexSrc_.c_str(), vertices);
   
   //Jets
   edm::Handle<edm::View<pat::Jet> > jets;
   iEvent.getByLabel(jetsSrc_.c_str(), jets);
   
   //Jets for Btag veto
   edm::Handle<edm::View<pat::Jet> > jets_btag_veto;
   iEvent.getByLabel(jets_btag_veto_Src_.c_str(), jets_btag_veto);
   
   //MET
   edm::Handle<edm::View<pat::MET> > metHandle;
   iEvent.getByLabel(metSrc_.c_str(), metHandle);
   
   //electrons
   edm::Handle<edm::View<reco::Candidate> > looseElectrons;
   iEvent.getByLabel(looseEleSrc_.c_str(), looseElectrons);
   
   //muons
   edm::Handle<edm::View<reco::Candidate> > looseMuons;
   iEvent.getByLabel(looseMuSrc_.c_str(), looseMuons); 
   
   //leptons (tight)
   edm::Handle<edm::View<reco::Candidate> > leptons;
   iEvent.getByLabel(leptonSrc_.c_str(), leptons); 
   nPV = vertices->size();
      
   //  Defining decay channel on the gen level
   N_had_Wgen  = 0, N_lep_Wgen = 0 ;

   DefineDecayChannel(genParticles, N_lep_Wgen , N_had_Wgen );
   
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
    m_pruned = hadronicV.userFloat("ak8PFJetsCHSPrunedLinks");   

    tau1 = hadronicV.userFloat("Njettiness:tau1");
    tau2 = hadronicV.userFloat("Njettiness:tau2");
    tau3 = hadronicV.userFloat("Njettiness:tau3");
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
   
   //MET quantities   
   const pat::MET& metCand = metHandle->at(0);
   
   METCand.pt = metCand.pt();
   METCand.phi = metCand.phi();
   METCand.mass = metCand.mass();
   METCand.mt = metCand.mt();
   
  NAK8jet = jets -> size();

   
   if (jets -> size() > 0)
  {
    jet_pt = (jets -> at(0)).pt();
    jet_eta = (jets -> at(0)).eta();
    jet_phi = (jets -> at(0)).phi();
    jet_mass_pruned = (jets -> at(0)).userFloat("ak8PFJetsCHSPrunedLinks");
    jet_mass_softdrop = (jets -> at(0)).userFloat("ak8PFJetsCHSSoftDropLinks");
    jet_tau2tau1 = ((jets -> at(0)).userFloat("Njettiness:tau2"))/((jets -> at(0)).userFloat("Njettiness:tau1"));
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
  njets = jets_btag_veto -> size(); 
  nbtag = 0;
  
  for (unsigned int iBtag = 0; iBtag < jets_btag_veto -> size(); iBtag ++)
  {
    //WP for 8 TeV and preliminary. Should be updated at some point
    if(((jets_btag_veto -> at(iBtag)).bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags")) > 0.814) nbtag ++;
  }
  
  if (jets_btag_veto -> size() > 0)
  {
    jet2_pt = (jets_btag_veto -> at(0)).pt();
    jet2_btag = (jets_btag_veto -> at(0)).bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags");
  }
  
  else 
  {
    jet2_pt = -99.;
    jet2_btag = -99.;
  }
  
  if (jets_btag_veto -> size() > 1)
  {
    jet3_pt = (jets_btag_veto -> at(1)).pt();
    jet3_btag = (jets_btag_veto -> at(1)).bDiscriminator("combinedInclusiveSecondaryVertexV2BJetTags");
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
   if (leptonicVs -> size() > 0)  m_lvj = lvj_p4.M();
   else m_lvj = -99.;


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
