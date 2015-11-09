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
#include "DataFormats/MuonReco/interface/Muon.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"


#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include "DecayChannel.h"
#include "DecayClass.h"
#include "Particle.h"
#include "SystematicsHelper.h"
#include "PU.h"

namespace reco {
  typedef edm::Ptr<reco::Muon> MuonPtr;
}
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

  double PUweight;

  double genWeight;
  double rho_;
  
  Particle Wboson_lep, METCand, Electron, Muon, Lepton;
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
  double jet_pt, jet_eta, jet_phi, jet_mass, jet_mass_pruned, jet_mass_softdrop, jet_tau2tau1;

  //JEC uncertainties
  double JECunc;
  double jet_pt_JECUp, jet_pt_JECDown, jet_mass_JECUp, jet_mass_JECDown, jet_mass_pruned_JECUp, jet_mass_pruned_JECDown, jet_mass_softdrop_JECUp, jet_mass_softdrop_JECDown;
  
  //AK4 jets
  double jet2_pt, jet2_btag, jet3_pt, jet3_btag;

  //MET uncertainties
  double MET_UnclEnUp, MET_UnclEnDown, MET_JECUp, MET_JECDown, MET_JERUp, MET_JERDown,  MET_LeptonEnUp, MET_LeptonEnDown;
  //MET phi uncertainties
  double MET_Phi_UnclEnUp, MET_Phi_UnclEnDown, MET_Phi_JECUp, MET_Phi_JECDown, MET_Phi_JERUp, MET_Phi_JERDown, MET_Phi_LeptonEnUp, MET_Phi_LeptonEnDown;
  
  //m_lvj
  double m_lvj;
  //m_lvj systematics
  double m_lvj_UnclEnUp, m_lvj_UnclEnDown, m_lvj_JECUp, m_lvj_JECDown, m_lvj_LeptonEnUp, m_lvj_LeptonEnDown, m_lvj_LeptonResUp, m_lvj_LeptonResDown;
  
  //Defining Tokens
  edm::EDGetTokenT<std::vector< PileupSummaryInfo > > PUInfoToken_;
  edm::EDGetTokenT<edm::View<pat::MET> > metToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> leptonicVToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> genParticlesToken_;
  edm::EDGetTokenT<edm::View<pat::Jet>> fatJetsToken_;
  edm::EDGetTokenT<edm::View<pat::Jet>> AK4JetsToken_;
  edm::EDGetTokenT<edm::View<reco::Vertex> > vertexToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> looseMuToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> looseEleToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> leptonsToken_;
  edm::EDGetTokenT<edm::View<pat::Muon>> muonsToken_;
  bool isMC;
  edm::EDGetTokenT<double> rhoToken_;
  std::string channel;
  edm::LumiReWeighting  LumiWeights_;
  edm::EDGetTokenT<GenEventInfoProduct> genInfoToken;
 
  //for JEC
  boost::shared_ptr<FactorizedJetCorrector> jecAK8_;
  SystematicsHelper SystematicsHelper_;

};

//
// constructors and destructor
//
TreeMaker::TreeMaker(const edm::ParameterSet& iConfig):
  metToken_(consumes<edm::View<pat::MET>>(iConfig.getParameter<edm::InputTag>("metSrc"))),
  leptonicVToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonicVSrc"))),
  genParticlesToken_(mayConsume<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("genSrc"))),
  fatJetsToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("fatJetSrc"))),
  AK4JetsToken_(consumes<edm::View<pat::Jet>>(iConfig.getParameter<edm::InputTag>("AK4JetSrc"))),
  vertexToken_(consumes<edm::View<reco::Vertex>>(iConfig.getParameter<edm::InputTag>("vertexSrc"))),
  looseMuToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("looseMuSrc"))),
  looseEleToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("looseEleSrc"))),
  leptonsToken_(consumes<edm::View<reco::Candidate>>(iConfig.getParameter<edm::InputTag>("leptonSrc"))),
  muonsToken_(mayConsume<edm::View<pat::Muon>>(iConfig.getParameter<edm::InputTag>("leptonSrc"))),
  isMC(iConfig.getParameter<bool>("isMC")),
  rhoToken_(consumes<double> (iConfig.getParameter<edm::InputTag>("rho"))),
  channel(iConfig.getParameter<std::string>("channel")),
  SystematicsHelper_(SystematicsHelper())
{
  //loading JEC from text files, this is done because groomed mass should be corrected with L2L3 corrections, if this is temporary, that shouldn't be done, as we take corrections from GT
  edm::FileInPath L2("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_MC_L2Relative_AK8PFchs.txt");
  edm::FileInPath L3("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_MC_L3Absolute_AK8PFchs.txt");
  edm::FileInPath L2Data("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_DATA_L2Relative_AK8PFchs.txt");
  edm::FileInPath L3Data("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_DATA_L3Absolute_AK8PFchs.txt");
  edm::FileInPath L2L3Res("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_DATA_L2L3Residual_AK8PFchs.txt"); 
  std::vector<std::string> jecAK8PayloadNames_;
  if (isMC){
    jecAK8PayloadNames_.push_back(L2.fullPath());
    jecAK8PayloadNames_.push_back(L3.fullPath()); 
    SystematicsHelper_  = SystematicsHelper(channel, consumesCollector());
  }

  if(!isMC) {
    jecAK8PayloadNames_.push_back(L2Data.fullPath()); 
    jecAK8PayloadNames_.push_back(L3Data.fullPath()); 
    jecAK8PayloadNames_.push_back(L2L3Res.fullPath()); 
  }

  std::vector<JetCorrectorParameters> vPar;
  for ( std::vector<std::string>::const_iterator payloadBegin = jecAK8PayloadNames_.begin(), payloadEnd = jecAK8PayloadNames_.end(), ipayload = payloadBegin; ipayload != payloadEnd; ++ipayload ) {
    JetCorrectorParameters pars(*ipayload);
    vPar.push_back(pars);
  }
  
  // Make the FactorizedJetCorrector
  jecAK8_ = boost::shared_ptr<FactorizedJetCorrector> ( new FactorizedJetCorrector(vPar) );


  //loading PU and generator information for MC
   if (isMC) {
     PUInfoToken_ = consumes<std::vector< PileupSummaryInfo > >(iConfig.getParameter<edm::InputTag>("PUInfo"));

     //PU-reweighting
     LumiWeights_ = edm::LumiReWeighting(MC_dist(), data_dist());

    genInfoToken = mayConsume<GenEventInfoProduct> (iConfig.getParameter<edm::InputTag>( "genInfo" ) );
   }

 
  //now do what ever initialization is needed
  edm::Service<TFileService> fs;
  outTree_ = fs->make<TTree>("BasicTree","BasicTree");

  //event info
  outTree_->Branch("event",	      &nevent,    	  "event/I"           );
  outTree_->Branch("lumi", 	      &lumi,   		  "lumi/I"  		);
  outTree_->Branch("run",	      &run,		  "run/I"  	       );
  
  //number of primary vertices
  outTree_->Branch("nPV",	      &nPV,		  "nPV/I"  	       );
  outTree_->Branch("rho",       &rho_,     "rho/D"          );
  //PUweight
  if (isMC) {
     outTree_->Branch("PUweight",       &PUweight,     "PUweight/D"          );
     outTree_->Branch("genWeight",       &genWeight,     "genWeight/D"          );
   };
  
  //number of loose leptons
  outTree_->Branch("nLooseEle",      &nLooseEle, 	  "nLooseEle/I"       );
  outTree_->Branch("nLooseMu",      &nLooseMu, 	  "nLooseMu/I"       );
  
  // number of leptons for particular mu/ele channel (should be tight lepton)
  outTree_->Branch("nLep",            &nLep, 	      "nLep/I"       );
  
  //leptons (tight, ele or mu depends on the channel)
  outTree_->Branch("l_pt",	      &Lepton.pt,      "l_pt/D"         	);
  outTree_->Branch("l_eta",	      &Lepton.eta,     "l_eta/D"        	);
  outTree_->Branch("l_phi",	      &Lepton.phi,     "l_phi/D"        	);
  //lepton uncertainties
  if (isMC) {
    outTree_->Branch("l_pt_EnUp",       &Lepton.pt_EnUp,     "l_pt_EnUp/D"          );
    outTree_->Branch("l_pt_EnDown",     &Lepton.pt_EnDown,   "l_pt_EnDown/D"          );
    outTree_->Branch("l_pt_ResUp",      &Lepton.pt_ResUp,    "l_pt_ResUp/D"          );
    outTree_->Branch("l_pt_ResDown",    &Lepton.pt_ResDown,  "l_pt_ResDown/D"          );
  }
  //W leptonic observables
  outTree_->Branch("W_pt",	      &Wboson_lep.pt,     "W_pt/D"         );
  outTree_->Branch("W_eta",	      &Wboson_lep.eta,    "W_eta/D"        );
  outTree_->Branch("W_phi",	      &Wboson_lep.phi,    "W_phi/D"        );

  
  outTree_->Branch("mass_W_lep",      &Wboson_lep.mass,   "mass_W_lep/D"       );

  
  outTree_->Branch("mt_W_lep",	      &Wboson_lep.mt,     "mt_W_lep/D"         );

  
  outTree_->Branch("charge_W_lep",    &Wboson_lep.charge, "charge_W_lep/D"     );
    
  outTree_->Branch("N_lep_W",	      &N_lep_W,		  "N_lep_W/I"          );
  
  outTree_->Branch("WDecayClass",     &WDecayClass,	  "WDecayClass/I"      );
  
  outTree_->Branch("N_had_W_gen",     &N_had_Wgen,	  "N_had_W_gen/I"      );
  outTree_->Branch("N_lep_W_gen",     &N_lep_Wgen, 	  "N_lep_W_gen/I"      );

  //MET observables  
  outTree_->Branch("pfMET", 	      &METCand.pt, 	  "pfMET/D"              );
  outTree_->Branch("pfMETPhi",	      &METCand.phi, 	  "pfMETPhi/D"          );
  outTree_->Branch("pfMETMass",	      &METCand.mass, 	  "pfMETMass/D"         );
  outTree_->Branch("pfMETMt",	      &METCand.mt, 	  "pfMETMt/D"           );

  if (isMC) {
    //MET uncertainties
    //UnclEn
    outTree_->Branch("pfMET_UnclEnUp",         &MET_UnclEnUp,    "pfMET_UnclEnUp/D"              );
    outTree_->Branch("pfMET_UnclEnDown",         &MET_UnclEnDown,    "pfMET_UnclEnDown/D"              );
    //JER
    outTree_->Branch("pfMET_JERUp",         &MET_JERUp,    "pfMET_JERUp/D"              );
    outTree_->Branch("pfMET_JERDown",         &MET_JERDown,    "pfMET_JERDown/D"              );
    //JEC
    outTree_->Branch("pfMET_JECUp",         &MET_JECUp,    "pfMET_JECUp/D"              );
    outTree_->Branch("pfMET_JECDown",         &MET_JECDown,    "pfMET_JECDown/D"              );
    //Lepton energy energy scale
    outTree_->Branch("pfMET_LeptonEnUp",         &MET_LeptonEnUp,    "pfMET_LeptonEnUp/D"              );
    outTree_->Branch("pfMET_LeptonEnDown",         &MET_LeptonEnDown,    "pfMET_LeptonEnDown/D"              );
   
    //MET phi uncertainties
    //UnclEn
    outTree_->Branch("pfMETPhi_UnclEnUp",         &MET_Phi_UnclEnUp,    "pfMETPhi_UnclEnUp/D"              );
    outTree_->Branch("pfMETPhi_UnclEnDown",         &MET_Phi_UnclEnDown,    "pfMETPhi_UnclEnDown/D"              );
    //JER
    outTree_->Branch("pfMETPhi_JERUp",         &MET_Phi_JERUp,    "pfMETPhi_JERUp/D"              );
    outTree_->Branch("pfMETPhi_JERDown",         &MET_Phi_JERDown,    "pfMETPhi_JERDown/D"              );
    //JEC
    outTree_->Branch("pfMETPhi_JECUp",         &MET_Phi_JECUp,    "pfMETPhi_JECUp/D"              );
    outTree_->Branch("pfMETPhi_JECDown",         &MET_Phi_JECDown,    "pfMETPhi_JECDown/D"              );
    //Lepton energy scale
    outTree_->Branch("pfMETPhi_LeptonEnUp",         &MET_Phi_LeptonEnUp,    "pfMETPhi_LeptonEnUp/D"              );
    outTree_->Branch("pfMETPhi_LeptonEnDown",         &MET_Phi_LeptonEnDown,    "pfMETPhi_LeptonEnDown/D"              );
   
  }
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
  outTree_->Branch("jet_mass",         &jet_mass,       "jet_mass/D"   );
  outTree_->Branch("jet_mass_pruned", &jet_mass_pruned,	  "jet_mass_pruned/D"   );
  outTree_->Branch("jet_mass_softdrop",&jet_mass_softdrop,"jet_mass_softdrop/D"   );
  outTree_->Branch("jet_tau2tau1",    &jet_tau2tau1,	  "jet_tau2tau1/D"   );

  //JEC uncertainties
  if (isMC) {
    outTree_->Branch("JECunc",    &JECunc,    "JECunc/D"   ); 
    outTree_->Branch("jet_pt_JECUp",    &jet_pt_JECUp,    "jet_pt_JECUp/D"   ); 
    outTree_->Branch("jet_pt_JECDown",    &jet_pt_JECDown,    "jet_pt_JECDown/D"   );  
    outTree_->Branch("jet_mass_JECUp",    &jet_mass_JECUp,    "jet_mass_JECUp/D"   ); 
    outTree_->Branch("jet_mass_JECDown",    &jet_mass_JECDown,    "jet_mass_JECDown/D"   );  
    
    outTree_->Branch("jet_mass_pruned_JECUp",    &jet_mass_pruned_JECUp,    "jet_mass_pruned_JECUp/D"   ); 
    outTree_->Branch("jet_mass_pruned_JECDown",    &jet_mass_pruned_JECDown,    "jet_mass_pruned_JECDown/D"   );  
    
    outTree_->Branch("jet_mass_softdrop_JECUp",    &jet_mass_softdrop_JECUp,    "jet_mass_softdrop_JECUp/D"   ); 
    outTree_->Branch("jet_mass_softdrop_JECDown",    &jet_mass_softdrop_JECDown,    "jet_mass_softdrop_JECDown/D"   );  
  }
  outTree_->Branch("njets",  	      &njets,	          "njets/I"   );
  outTree_->Branch("nbtag",  	      &nbtag,	          "nbtag/I"   );
  
  outTree_->Branch("jet2_pt",  	      &jet2_pt,	          "jet2_pt/D"   );
  outTree_->Branch("jet2_btag",       &jet2_btag,         "jet2_btag/D"   );
  outTree_->Branch("jet3_pt",  	      &jet3_pt,	          "jet3_pt/D"   );
  outTree_->Branch("jet3_btag",	      &jet3_btag,         "jet3_btag/D"   );
  
  outTree_->Branch("m_lvj",	      &m_lvj,         "m_lvj/D"   );
  if (isMC) {
    outTree_->Branch("m_lvj_UnclEnUp",       &m_lvj_UnclEnUp,         "m_lvj_UnclEnUp/D"   );
    outTree_->Branch("m_lvj_UnclEnDown",       &m_lvj_UnclEnDown,         "m_lvj_UnclEnDown/D"   );      
    outTree_->Branch("m_lvj_JECUp",       &m_lvj_JECUp,         "m_lvj_JECUp/D"   );
    outTree_->Branch("m_lvj_JECDown",       &m_lvj_JECDown,         "m_lvj_JECDown/D"   );  
    outTree_->Branch("m_lvj_LeptonEnUp",       &m_lvj_LeptonEnUp,         "m_lvj_LeptonEnUp/D"   );
    outTree_->Branch("m_lvj_LeptonEnDown",       &m_lvj_LeptonEnDown,         "m_lvj_LeptonEnDown/D"   );      
    outTree_->Branch("m_lvj_LeptonResUp",       &m_lvj_LeptonResUp,         "m_lvj_LeptonResUp/D"   );
    outTree_->Branch("m_lvj_LeptonResDown",       &m_lvj_LeptonResDown,         "m_lvj_LeptonResDown/D"   );      
  }



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

   edm::Handle< double > rhoH;
   iEvent.getByToken(rhoToken_,rhoH);
   rho_ = *rhoH;
      
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

   std::map<std::string, math::XYZTLorentzVector>  SystMap; 
   std::map<std::string, math::XYZTLorentzVector>  LeptonSystMap;
   if (isMC) {
      SystMap = SystematicsHelper_.getWLepSystematicsLoretzVectors(iEvent);
      LeptonSystMap = SystematicsHelper_.getLeptonSystematicsLoretzVectors(iEvent);
   }

   nPV = vertices->size();
   
   edm::Handle<std::vector< PileupSummaryInfo > >  PUInfo;
   edm::Handle <GenEventInfoProduct> genInfo; 
  if (isMC) {
    iEvent.getByToken(PUInfoToken_, PUInfo);
    std::vector<PileupSummaryInfo>::const_iterator PVI;
    float Tnpv = -1;
    for(PVI = PUInfo->begin(); PVI != PUInfo->end(); ++PVI) {
     int BX = PVI->getBunchCrossing();
     if(BX == 0) { 
       Tnpv = PVI->getTrueNumInteractions();
       continue;
     }

    }
    PUweight = LumiWeights_.weight( Tnpv );

   iEvent.getByToken( genInfoToken , genInfo);
   genWeight = (genInfo -> weight());
  }
  /*edm::ESHandle<JetCorrectorParametersCollection> JetCorParCollAK8;
  iSetup.get<JetCorrectionsRecord>().get("AK8PFchs",JetCorParCollAK8);

  JetCorrectorParameters const & JetCorPar = (*JetCorParCollAK8)["Uncertainty"];
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JetCorPar);*/
  //edm::FileInPath JECUncertainty("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_MC_Uncertainty_AK8PFchs.txt");
  edm::FileInPath JECUncertainty("aTGCsAnalysis/TreeMaker/data/Summer15_25nsV5_MC_Uncertainty_AK8PFchs.txt");
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(JECUncertainty.fullPath());

  jecUnc->setJetEta((jets -> at(0)).eta());
  jecUnc->setJetPt((jets -> at(0)).pt()); // here you must use the CORRECTED jet pt
 
  JECunc = jecUnc->getUncertainty(true);


   
      
   //  Defining decay channel on the gen level
   N_had_Wgen  = 0, N_lep_Wgen = 0 ;

   if (isMC) DefineDecayChannel(genParticles, N_lep_Wgen , N_had_Wgen );
   
   if (N_lep_Wgen == 0 && N_had_Wgen == 2  ) WDecayClass = Hadronic;
   else if (N_lep_Wgen == 1 && N_had_Wgen == 1 ) WDecayClass = Semileptonic;
   else if (N_lep_Wgen == 2 && N_had_Wgen == 0 ) WDecayClass = Leptonic;
   else WDecayClass = UnDefined;

   N_lep_W = leptonicVs -> size();
   
   //loose leptons
   nLooseEle = looseElectrons -> size();
   nLooseMu = looseMuons -> size();
   
   nLep = leptons -> size();
   
   auto leptonPtr = leptons -> ptrAt(0);
   reco::MuonPtr asmuonPtr(leptonPtr);
   //electron channel
   if ( ( leptons -> size() ) > 0)
   {
     if (channel == "mu"){
      Lepton.pt =  asmuonPtr -> tunePMuonBestTrack() -> pt();
      Lepton.eta = asmuonPtr -> tunePMuonBestTrack() -> eta();
      Lepton.phi = asmuonPtr -> tunePMuonBestTrack() -> phi();
    }
    else if (channel == "el"){
      Lepton.pt = (leptons -> at(0)).pt();
      Lepton.eta = (leptons -> at(0)).eta();
      Lepton.phi = (leptons -> at(0)).phi();
    }
    else {
      std::cerr << "Invalid channel used, use el or mu." << std::endl;
      exit(0);
    }
     if (isMC){
       Lepton.pt_EnUp = LeptonSystMap.at("LeptonEnUp").Pt();
       Lepton.pt_EnDown = LeptonSystMap.at("LeptonEnDown").Pt();
       Lepton.pt_ResUp = LeptonSystMap.at("LeptonResUp").Pt();
       Lepton.pt_ResDown = LeptonSystMap.at("LeptonResDown").Pt();
    }
   }

    
   else 
   {
     Lepton.pt = -99.;
     Lepton.eta = -99.;
     Lepton.phi = -99.;
     Lepton.pt_EnUp = -99.;
     Lepton.pt_EnDown = -99.;
     Lepton.pt_ResUp = -99.;
     Lepton.pt_ResDown = -99.;
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
  
    //MET quantities   
   if (metHandle->size() > 0)
   {
      const pat::MET& metCand = metHandle->at(0);
      
      METCand.pt = metCand.pt();
      METCand.phi = metCand.phi();
      METCand.mass = metCand.mass();
      METCand.mt = metCand.mt();
      //MET uncertainties
      //METUncl
      if (isMC){
          MET_UnclEnUp = metCand.shiftedPt( pat::MET::UnclusteredEnUp, pat::MET::Type1) ;
          MET_UnclEnDown = metCand.shiftedPt( pat::MET::UnclusteredEnDown, pat::MET::Type1) ;
          //JER
          MET_JERUp = metCand.shiftedPt( pat::MET::JetResUp, pat::MET::Type1) ;
          MET_JERDown = metCand.shiftedPt( pat::MET::JetResDown, pat::MET::Type1) ;
          //JEC
          MET_JECUp = metCand.shiftedPt( pat::MET::JetEnUp, pat::MET::Type1) ;
          MET_JECDown = metCand.shiftedPt( pat::MET::JetEnDown, pat::MET::Type1) ;
          //Electron energy 
          if( channel == "el") {
            MET_LeptonEnUp = metCand.shiftedPt( pat::MET::ElectronEnUp, pat::MET::Type1) ;
            MET_LeptonEnDown = metCand.shiftedPt( pat::MET::ElectronEnDown, pat::MET::Type1) ;
          }
          //Muon energy 
          else if (channel == "mu"){
            MET_LeptonEnUp = metCand.shiftedPt( pat::MET::MuonEnUp, pat::MET::Type1) ;
            MET_LeptonEnDown = metCand.shiftedPt( pat::MET::MuonEnDown, pat::MET::Type1) ;
          }
          
          else {
            std::cerr << "Invalid channel used, please use mu or el." << std::endl;
            exit(0);
          }  
          //MET phi uncertainties
          //METUncl
          MET_Phi_UnclEnUp = metCand.shiftedPhi( pat::MET::UnclusteredEnUp, pat::MET::Type1) ;
          MET_Phi_UnclEnDown = metCand.shiftedPhi( pat::MET::UnclusteredEnDown, pat::MET::Type1) ;
          //JER
          MET_Phi_JERUp = metCand.shiftedPhi( pat::MET::JetResUp, pat::MET::Type1) ;
          MET_Phi_JERDown = metCand.shiftedPhi( pat::MET::JetResDown, pat::MET::Type1) ;
          //JEC
          MET_Phi_JECUp = metCand.shiftedPhi( pat::MET::JetEnUp, pat::MET::Type1) ;
          MET_Phi_JECDown = metCand.shiftedPhi( pat::MET::JetEnDown, pat::MET::Type1) ;
          //Lepton energy 
          if (channel == "el"){
            MET_Phi_LeptonEnUp = metCand.shiftedPhi( pat::MET::ElectronEnUp, pat::MET::Type1) ;
            MET_Phi_LeptonEnDown = metCand.shiftedPhi( pat::MET::ElectronEnDown, pat::MET::Type1) ;
          }
          else if (channel == "mu"){
            MET_Phi_LeptonEnUp = metCand.shiftedPhi( pat::MET::MuonEnUp, pat::MET::Type1) ;
            MET_Phi_LeptonEnDown = metCand.shiftedPhi( pat::MET::MuonEnDown, pat::MET::Type1) ;
        }
         else {
            std::cerr << "Invalid channel used, please use mu or el." << std::endl;
           exit(0);
          }  
      } 
   }
   
    else
   {
      METCand.pt = -99.;
      METCand.phi = -99.;
      METCand.mass = -99.;
      METCand.mt = -99.;

      //MET uncertainties
       //METUncl
      MET_UnclEnUp = -99. ;
      MET_UnclEnUp = -99.;
      //JER
      MET_JERUp = -99.;
      MET_JERDown = -99. ;
      //JEC
      MET_JECUp = -99. ;
      MET_JECDown = -99. ;
      //Lepton energy 
      MET_LeptonEnUp = -99.;
      MET_LeptonEnDown = -99. ;

      //MET phi uncertainties
       //METUncl
      MET_Phi_UnclEnUp = -99. ;
      MET_Phi_UnclEnUp = -99.;
      //JER
      MET_Phi_JERUp = -99.;
      MET_Phi_JERDown = -99. ;
      //JEC
      MET_Phi_JECUp = -99. ;
      MET_Phi_JECDown = -99. ;
      //Lepton energy 
      MET_Phi_LeptonEnUp = -99.;
      MET_Phi_LeptonEnDown = -99. ;
      
   }
   
    if (jets -> size() > 0 && leptonicVs -> size() > 0)
    {
      deltaR_LepWJet = deltaR(Lepton.eta,Lepton.phi,(jets -> at(0)).eta(), (jets -> at(0)).phi()); 
      deltaPhi_LepMet = deltaPhi(Lepton.phi, METCand.phi);
      deltaPhi_WJetMet = deltaPhi((jets -> at(0)).phi(), METCand.phi);
      deltaPhi_WJetWlep = deltaPhi((jets -> at(0)).phi(), Wboson_lep.phi);
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
    jet_mass = (jets -> at(0)).mass();
    //jet_mass_pruned = (jets -> at(0)).userFloat("ak8PFJetsCHSPrunedMass");
    //jet_mass_softdrop = (jets -> at(0)).userFloat("ak8PFJetsCHSSoftDropMass");
    jet_tau2tau1 = ((jets -> at(0)).userFloat("NjettinessAK8:tau2"))/((jets -> at(0)).userFloat("NjettinessAK8:tau1"));

    math::XYZTLorentzVector uncorrJet = (jets -> at(0)).correctedP4(0);
    jecAK8_->setJetEta( uncorrJet.eta() );
    jecAK8_->setJetPt ( uncorrJet.pt() );
    jecAK8_->setJetE  ( uncorrJet.energy() );
    jecAK8_->setJetA  ( (jets -> at(0)).jetArea() );
    jecAK8_->setRho   ( rho_ );
    jecAK8_->setNPV   (  vertices->size());
    double corr = jecAK8_->getCorrection();
    jet_mass_pruned = corr*(jets -> at(0)).userFloat("ak8PFJetsCHSPrunedMass");
    jet_mass_softdrop = corr*(jets -> at(0)).userFloat("ak8PFJetsCHSSoftDropMass");

    //JEC uncertainty
    jet_pt_JECDown = (1 - JECunc)*jet_pt;
    jet_pt_JECUp   = (1 + JECunc)*jet_pt;
    jet_mass_JECDown = (1 - JECunc)*jet_mass;
    jet_mass_JECUp   = (1 + JECunc)*jet_mass;
    jet_mass_pruned_JECDown = (1 - JECunc)*jet_mass_pruned; 
    jet_mass_pruned_JECUp = (1 + JECunc)*jet_mass_pruned; 
    jet_mass_softdrop_JECDown = (1 - JECunc)*jet_mass_softdrop; 
    jet_mass_softdrop_JECUp = (1 + JECunc)*jet_mass_softdrop; 

  }
  
  else 
  {
    jet_pt = -99.;
    jet_eta = -99.;
    jet_phi = -99.;
    jet_mass_pruned = -99.;
    jet_mass_softdrop = -99.;
    jet_tau2tau1 = -99.;

    //JEC Uncertainty
    jet_pt_JECDown = -99.;
    jet_pt_JECUp   = -99.;
    jet_mass_JECDown = -99.;
    jet_mass_JECUp   = -99.;
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
   ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > hadronicVp4, leptonicVp4, lvj_p4;
   //hadronic W
   hadronicVp4.SetPt((jets -> at(0)).pt());
   hadronicVp4.SetEta((jets -> at(0)).eta());
   hadronicVp4.SetPhi((jets -> at(0)).phi());
   hadronicVp4.SetM((jets -> at(0)).mass());
   //leptonic W
   leptonicVp4.SetPt(Wboson_lep.pt);
   leptonicVp4.SetEta(Wboson_lep.eta);
   leptonicVp4.SetPhi(Wboson_lep.phi);
   leptonicVp4.SetM(Wboson_lep.mass);

   
   lvj_p4 = hadronicVp4 + leptonicVp4;
   if (leptonicVs -> size() > 0 && jets -> size() > 0)   m_lvj = lvj_p4.M();
   else m_lvj = -99.;
   //systematics
   //METUnclEn
   math::XYZTLorentzVector lvj_p4_Up, lvj_p4_Down;
   if (leptonicVs -> size() > 0 && jets -> size() > 0 && isMC)  {
     lvj_p4_Up = hadronicVp4 + SystMap.at("UnclusteredEnUp");
     lvj_p4_Down = hadronicVp4 + SystMap.at("UnclusteredEnDown");
     m_lvj_UnclEnUp = lvj_p4_Up.M();
     m_lvj_UnclEnDown = lvj_p4_Down.M();
   }
   else {
     m_lvj_UnclEnUp = -99.;
     m_lvj_UnclEnDown = -99.;
   }
   //JEC
    math::XYZTLorentzVector hadronicVp4_Up, hadronicVp4_Down;
    hadronicVp4_Up = hadronicVp4;
    hadronicVp4_Down = hadronicVp4;
    hadronicVp4_Up *= (1+JECunc); 
    hadronicVp4_Down *= (1-JECunc);
   if (leptonicVs -> size() > 0 && jets -> size() > 0 && isMC)  {
     lvj_p4_Up = hadronicVp4_Up + SystMap.at("JetEnUp");
     lvj_p4_Down = hadronicVp4_Down + SystMap.at("JetEnDown");
     m_lvj_JECUp = lvj_p4_Up.M();
     m_lvj_JECDown = lvj_p4_Down.M();
   }
   else {
     m_lvj_JECUp = -99.;
     m_lvj_JECDown = -99.;
   }
   //lepton energy scale uncertainty
   if (leptonicVs -> size() > 0 && jets -> size() > 0 && isMC)  {
    lvj_p4_Up = hadronicVp4 + SystMap.at("LeptonEnUp");
    lvj_p4_Down = hadronicVp4 + SystMap.at("LeptonEnDown");
    m_lvj_LeptonEnUp = lvj_p4_Up.M();
    m_lvj_LeptonEnDown = lvj_p4_Down.M();

   }
   else {
    m_lvj_LeptonEnUp = -99.;
    m_lvj_LeptonEnDown = -99.;
   }

   //lepton energy resolution uncertainty
   if (leptonicVs -> size() > 0 && jets -> size() > 0 && isMC)  {
    lvj_p4_Up = hadronicVp4 + SystMap.at("LeptonResUp");
    lvj_p4_Down = hadronicVp4 + SystMap.at("LeptonResDown");
    m_lvj_LeptonResUp = lvj_p4_Up.M();
    m_lvj_LeptonResDown = lvj_p4_Down.M();

   }
   else {
    m_lvj_LeptonResUp = -99.;
    m_lvj_LeptonResDown = -99.;
   }

 // uncomment the line used in the synchronization exercise!
 if (deltaR_LepWJet > (TMath::Pi()/2.0) && fabs(deltaPhi_WJetMet) > 2. && fabs(deltaPhi_WJetWlep) > 2. && Wboson_lep.pt > 200.  && jet_tau2tau1 < 0.5) outTree_->Fill();
   //outTree_->Fill();



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
