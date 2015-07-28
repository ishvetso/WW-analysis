
/*
 * 
 * Function to define the decay of WW. Valid for GenParticles only.
 * 
 */
#ifndef Decay_Channel_h
#define Decay_Channel_h

 #include "DataFormats/Candidate/interface/Candidate.h"

inline void DefineDecayChannel ( edm::Handle<edm::View<reco::Candidate> > genParticles_, int & N_lep, int & N_had)
{
  std::vector <int> index_W;
  bool isLeptonic, isHadronic;
  int daughterPDG;
  
  for (unsigned int iGen = 0; iGen < genParticles_ -> size(); ++iGen)
  {
    if (fabs((genParticles_ -> at(iGen)).pdgId()) == 24) index_W.push_back(iGen);
  }
  
  //loop over Ws
  for (unsigned int iW = 0; iW < index_W.size(); iW ++)
  {
    //loop over W's daughters
    
   // std::cout << "N lep daughters: " << leptonicV.numberOfDaughters() << std::endl;
    for (unsigned int iDaughter = 0; iDaughter < (genParticles_ -> at(index_W.at(iW))).numberOfDaughters(); iDaughter ++)
    {
     if  ( fabs((((genParticles_ -> at(index_W.at(iW))).daughter(iDaughter)) -> mother()) -> pdgId()) == 24) 
     {
	  daughterPDG = fabs(((genParticles_ -> at(index_W.at(iW))).daughter(iDaughter)) -> pdgId());
	  if ( daughterPDG == 11 || daughterPDG == 13  ) isLeptonic = true;
	  else if ( daughterPDG == 1 || daughterPDG == 2 || daughterPDG == 3 || daughterPDG == 4 || daughterPDG == 5 || daughterPDG == 6) isHadronic = true;
	  else continue;
     }
      
    }// end of loop over W's daughters
    
    //std::cout << "isHadronic " << isHadronic << std::endl;
    //std::cout << "isLeptonic " << isLeptonic << std::endl;
    
    if (isHadronic) N_had ++ ; 
    if (isLeptonic) N_lep ++;
    isHadronic = false;
    isLeptonic = false;
    
  }//end of loop over Ws
  
  
  
}

#endif