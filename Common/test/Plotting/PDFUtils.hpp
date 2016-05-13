#ifndef  PDFUTILS
#define PDFUTILS
/*
* create an envelope for the list of histogram
*
*/
#include <algorithm>
#include "TH1D.h"
#include <iostream>
#include <string>
#include <stdexcept>


std::string random_string( size_t length );

TH1D * makeEnvelope(std::vector<TH1D*> hists, std::string variation);

TH1D * makePDF4LHC(std::vector<TH1D*> hists, std::string variation);
#endif
