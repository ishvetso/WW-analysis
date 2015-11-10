#ifndef PARTICLE_h
#define PARTICLE_h
struct Particle{
  double pt, eta, phi, mass, mt, charge;
  //uncertainties
  double pt_JECUp, pt_JECDown;
  double pt_LeptonEnUp, pt_LeptonEnDown;
  double pt_LeptonResUp, pt_LeptonResDown;
  double pt_UnclEnUp, pt_UnclEnDown;

  double mass_JECUp, mass_JECDown;
  double mass_LeptonEnUp, mass_LeptonEnDown;
  double mass_LeptonResUp, mass_LeptonResDown;
  double mass_UnclEnUp, mass_UnclEnDown;

  double mt_JECUp, mt_JECDown;
  double mt_LeptonEnUp, mt_LeptonEnDown;
  double mt_LeptonResUp, mt_LeptonResDown;
  double mt_UnclEnUp, mt_UnclEnDown;
};
#endif