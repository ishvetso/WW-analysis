#ifndef PDFVARIATIONMAP_H
#define PDFVARIATIONMAP_H

struct range 
{
  double low;
  double high;
  range (){};
  range(double low_, double high_):low(low_),high(high_){};
  
};

extern const std::map<int, range> PDFVariationMap = {
	{292200, range(2001,2102)},
	{263000, range(10,110)},
	{-1, range(2001,2100)},
	{292000, range(2001,2102)}
};

#endif
