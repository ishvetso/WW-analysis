
/*
 * 
 * Function to define the candidate is matched to the generator level W. 
 * 
 */
#ifndef isMatchedToGenW_h
#define isMatchedToGenW_h

 #include "DataFormats/Candidate/interface/Candidate.h"
 #include "DataFormats/Math/interface/deltaR.h"

template<class T > bool isMatchedToGenW ( edm::Handle<edm::View<reco::Candidate> > genParticles_, T cand)
{
  std::vector <int> index_W;
  bool isMatched = false;
  
  for (unsigned int iGen = 0; iGen < genParticles_ -> size(); ++iGen)
  {
    //status 22 in pythia8 corresponds to intermediate particle
    if (std::abs((genParticles_ -> at(iGen)).pdgId()) == 24 && (genParticles_ -> at(iGen)).status()==22) index_W.push_back(iGen);
  }
  
  //loop over Ws
  for (unsigned int iW = 0; iW < index_W.size(); iW ++)
  {
    double deltaR_ = deltaR(genParticles_ -> at(index_W.at(iW)), cand);
    if (deltaR_ < 0.8) isMatched = true;

  }//end of loop over Ws
  
  return isMatched;
  
}

 ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > genWLorentzVector ( edm::Handle<edm::View<reco::Candidate> > genParticles_, int charge)
{
  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > p4_;
  p4_.SetPt(-100.);
  p4_.SetEta(-100.);
  p4_.SetPhi(-100.);
  p4_.SetM(-100.);
  
  for (unsigned int iGen = 0; iGen < genParticles_ -> size(); ++iGen)
  {
    //status 22 in pythia8 corresponds to intermediate particle
    if (charge == 1 && (genParticles_ -> at(iGen)).pdgId() == 24 && (genParticles_ -> at(iGen)).status()==22) p4_ = genParticles_ -> at(iGen).p4();
    else if (charge == -1 && (genParticles_ -> at(iGen)).pdgId() == -24 && (genParticles_ -> at(iGen)).status()==22) p4_ = genParticles_ -> at(iGen).p4();
    else throw cms::Exception("InvalidValue") <<  "the value of the charge is not supported, please use only +1 or -1" << std::endl;

  }
  
  return p4_;
  
}

#endif