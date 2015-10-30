#ifndef PARTICLE_h
#define PARTICLE_h
struct Particle{
  double pt, eta, phi, mass, mt, charge;
  //uncertainties
  double pt_EnUp, pt_EnDown;
  double pt_ResUp, pt_ResDown;
};
#endif