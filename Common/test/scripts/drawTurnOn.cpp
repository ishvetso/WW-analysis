#include <TFile.h>
#include <TGraph.h>
#include <TPad.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <TLegend.h>
#include <TDirectoryFile.h>
#include <iostream>
#include <iostream>
#include </afs/cern.ch/work/i/ishvetso/aTGCRun2/for_thesis/CMSSW_7_6_4/src/aTGCsAnalysis/TreeMaker/plugins/Ele27WPLooseTrigTurnOn.h>

//a small macro to draw the turn on provided by Sam Harper for HLT_Ele27_WPLoose_Gsf measured for 2015


TGraph * turnOn(double etMin_, double etMax_, double etStep_, double eta){
   
   std::vector <double> etValues;
   std::vector <double> turnOnValues;

   double et = etMin_;

   while (et <=  etMax_) {
    etValues.push_back(et);
    turnOnValues.push_back(trigEle27NoER::turnOn(et, eta));
    et += etStep_;
   }
   
   TGraph* gr = new TGraph(etValues.size(),etValues.data(),turnOnValues.data());
   gr -> GetXaxis() -> SetRangeUser(etMin_, etMax_);

   return gr;
}

void drawTurnOn()
{
   TGraph * grBarrel = turnOn(25.,100., 1., 1.9);
   TGraph * grEndcaps = turnOn(25.,100., 1., 2.);

   grBarrel -> SetLineWidth(2.);
   grEndcaps -> SetLineWidth(2.);

   grBarrel -> SetLineColor(kBlue);
   grEndcaps -> SetLineColor(kRed);

   grBarrel -> GetYaxis() -> SetTitle("Efficiency");
   grBarrel -> GetXaxis() -> SetTitle("supercluster E_{T}[GeV]");
  
   TCanvas *c1= new TCanvas("c1","canvas",1200,800);
   TLegend *leg = new TLegend(0.6,0.4,0.7,0.6);

   gStyle->SetCanvasColor(kWhite);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);

   leg -> SetFillColor(kWhite);  
   leg -> SetBorderSize(0.);  

   leg->AddEntry(grBarrel, "barrel","l");
   leg->AddEntry(grEndcaps, "endcaps","l");

   grBarrel -> Draw();
   grEndcaps -> Draw("SAME");
   leg -> Draw("SAME");
   c1 -> SaveAs("turnOn-HLTEle27WPLoose.pdf");
};