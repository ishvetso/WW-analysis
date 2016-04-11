#include "Plotter.hpp"

#include "SystHelper.hpp"

Plotter::Plotter()
{
//do nothing
}


Plotter::Plotter(CHANNEL channel_)
{
	channel = channel_;
}


void Plotter::SetNbins(int Nbins_)
{
	Nbins = Nbins_;
}

void Plotter::SetSamples(vector <Sample> samples_)
{
	samples = samples_;
}

void Plotter::SetDataSample(Sample sample_)
{
	DataSample = sample_;
}

void Plotter::SetVar(vector <Var> variables_)
{
  
  variables = variables_;
}

void Plotter::Plotting(std::string OutPrefix_)
{
  system(("mkdir -p " + OutPrefix_ ).c_str());
  system(("mkdir -p " + OutPrefix_ + "/png").c_str());
  system(("mkdir -p " + OutPrefix_ + "/pdf").c_str());
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  setTDRStyle();
  // Prepare plots for the various variables 
  TCanvas* c1;
  TPad* pad1;
  TPad* pad2;
  std::map<std::string,TLegend*> leg;
  std::map<std::string,TH1D*> data;
  std::map<std::string,TH1D*> signalHist;
  std::map<std::string,THStack*> hs;
  std::map<std::string,TH1D*> hist_summed;
  std::map<std::pair<std::string,std::string>, TH1D*> hist_per_process;
  
  SystHelper systematics(samples[0].selection);
  
  for (uint var_i = 0; var_i < variables.size(); ++ var_i )   {
    
    std::string vname = variables.at(var_i).VarName;
    leg[vname] = new TLegend(0.8,0.5,0.98,0.93);
    leg[vname] ->  SetFillColor(kWhite);
    
    data[vname] = new TH1D((DataSample.Processname + variables.at(var_i).VarName + "_data").c_str(),(DataSample.Processname + variables.at(var_i).VarName + "_data").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
    data[vname] -> Sumw2();
    
    signalHist[vname] = new TH1D(("signal_" + variables.at(var_i).VarName ).c_str(),("signal_" + variables.at(var_i).VarName ).c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
    signalHist[vname]-> Sumw2();
    //for Monte-Carlo samples
    hs[vname] = new THStack("hs",(";"+ vname +";Number of events").c_str());	  
    // sum of all processes for a given variable
    hist_summed[vname] = new TH1D((vname + "summed").c_str(),( vname+ "summed").c_str(), Nbins, variables.at(var_i).Range.low, variables.at(var_i).Range.high);
    hist_summed[vname] -> Sumw2();

    systematics.AddVar( &(variables.at(var_i)) ,  hist_summed[vname]);
  }


  //begin loop over data files
  for (uint file_i = 0; file_i < DataSample.filenames.size(); ++file_i){
    TFile file((DataSample.filenames.at(file_i)).c_str(), "READ");
    TTree * tree = (TTree*)file.Get("treeDumper/BasicTree");
    TTreeFormula *dataSelection = new TTreeFormula("dataSelection",DataSample.selection.c_str(),tree);//that should be without any weights!
    
    //initialize variables.
    for(auto var = variables.begin(); var != variables.end() ; var++){
      var->Initialize(tree);
    }
    //event loop
    Long64_t nentries = tree->GetEntriesFast();
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t nb = tree->GetEntry(jentry);
      if(nb<0) break; // reached the end of the ntuple
      
      // fill variables
      for(auto var = variables.begin(); var != variables.end() ; var++){
	std::string vname = var->VarName;
	if(dataSelection -> EvalInstance())data[vname]->Fill(var->value());//check if the event passeds the selection, and if true fill the histogram
      }
    }
  } // end of the loop over data files
  
  //begin the loop over signal files      
  for (uint file_i = 0; file_i < SignalSample.filenames.size(); ++file_i){ 
    TFile file((SignalSample.filenames.at(file_i)).c_str(), "READ");
    TTree * tree = (TTree*)file.Get("treeDumper/BasicTree");
    TTreeFormula *signalSelection = new TTreeFormula("signalSelection",SignalSample.selection.c_str(),tree);//that should be without any weights!
    double totWeight;
    std::vector<double> * aTGCWeights = 0;
    tree -> SetBranchAddress("totWeight", &totWeight);//for signal sample use weight without lumi weight
    tree -> SetBranchAddress("aTGCWeights", &aTGCWeights);
    
    //initialize variables.
    for(auto var = variables.begin(); var != variables.end() ; var++){
      var->Initialize(tree);
    }
    //event loop
    Long64_t nentries = tree->GetEntriesFast();
    for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t nb = tree->GetEntry(jentry);
      if(nb<0) break; // reached the end of the ntuple
      
      // fill variables
      for(auto var = variables.begin(); var != variables.end() ; var++){
	std::string vname = var->VarName;
 // std::cout << totWeight  << std::endl;
	if(signalSelection -> EvalInstance())signalHist[vname]->Fill(var->value(),totWeight*(aTGCWeights->at(1))*2300./20.);//check if the event passeds the selection, and if true fill the histogram
      }
    }
  }// end loop over signal files

  
  //Monte carlo samples
  //beginning of cycle over processes
  for (uint process_i = 0; process_i < samples.size(); process_i++){
    //create a buffer for the process		  	
    for(auto var = variables.begin(); var != variables.end() ; var++){
      std::string vname = var-> VarName;
      std::string process = samples[process_i].Processname;
      std::pair<std::string,std::string> key(vname,process);
      hist_per_process[key] = new TH1D((process + "_" + vname).c_str(),(process + "_" + vname).c_str(), Nbins,var->Range.low, var->Range.high);
      hist_per_process[key] -> Sumw2();
    }
    //loop over files for the given process
    for (uint file_i = 0; file_i < (samples.at(process_i)).filenames.size(); ++file_i){
      TFile file((samples.at(process_i)).filenames.at(file_i).c_str(), "READ");
      std::cout << (samples.at(process_i)).filenames.at(file_i) << std::endl; 
      TTree * tree = (TTree*)file.Get("BasicTree");
      Double_t totEventWeight;
      double genWeight, lumiWeight,PUWeight;
      std::vector<double> *PDFWeights = 0;
      tree -> SetBranchAddress("totEventWeight3", &totEventWeight);
      tree -> SetBranchAddress("PDFWeights", &PDFWeights);
      TTreeFormula *MCSampleSelection = new TTreeFormula("MCSampleSelection",samples.at(process_i).selection.c_str(),tree);//that should be without any weights!
      std::map<std::string, vector<TH1D*> *> histsPDF;
	  
      //initialize variables.
      for(auto var = variables.begin(); var != variables.end() ; var++)
      {
	       var->Initialize(tree);
         std::pair<std::string,std::string> key(var->VarName,std::string(""));

         for (uint iSyst = 0;iSyst < systematics.ListOfSystematics.size(); iSyst ++)
         {  
            key.second=systematics.ListOfSystematics.at(iSyst);  
            if (systematics.isAffectedBySystematic(*var, systematics.ListOfSystematics.at(iSyst))){
              Var *varUp = new Var();
              Var *varDown = new Var();
              varUp->VarName = (var -> VarName )+ "_" + systematics.ListOfSystematics.at(iSyst) + "Up";
              varDown->VarName = (var -> VarName )+ "_" + systematics.ListOfSystematics.at(iSyst) + "Down";
              varUp->Initialize(tree);
              varDown->Initialize(tree);
              SystematicsVarMapUp[key] = varUp;
              SystematicsVarMapDown[key] = varDown;
            }
            else {
              SystematicsVarMapUp[key] = &(*var);
              SystematicsVarMapDown[key] = &(*var);
            } 
         }
      }
      systematics.initTree(tree);

      //event loop
      Long64_t nentries = tree->GetEntriesFast();
      for (Long64_t jentry=0; jentry<nentries;jentry++) 
      {
	       Long64_t nb = tree->GetEntry(jentry);
         if(nb<0) break; // reached the end of the ntuple

         if (jentry == 0){
              for(auto var = variables.begin(); var!= variables.end(); var++)
              {
                for (uint iPDF =0; iPDF < PDFWeights -> size(); iPDF ++ )
                {
                  histsPDF[var->VarName]->at(iPDF) = new TH1D(("PDFhist" + var->VarName+ std::to_string(iPDF)).c_str(), ("PDFhist"+  var->VarName + std::to_string(iPDF)).c_str(), Nbins, var ->Range.low, var->Range.high);
                }  
              }
          }
	
	       // fill variables
          std::string process = samples[process_i].Processname;
          std::pair<std::string,std::string> key("",process);
         if(MCSampleSelection -> EvalInstance()) {
          for(auto var = variables.begin(); var != variables.end() ; var++)
          {
      	     key.first = var -> VarName;
      	     hist_per_process[key]->Fill(var->value(), totEventWeight);//check if the event passeds the selection, and if true fill the histogram
              for (uint iPDF =0; iPDF < PDFWeights -> size(); iPDF ++ )
              {
                  histsPDF[var->VarName]->at(iPDF) -> Fill(var->value(), totEventWeight*PDFWeights->at(iPDF));
              }  
	         }
	       }
       systematics.fill(&variables, SystematicsVarMapUp, SystematicsVarMapDown,totEventWeight);

      }
    }// end of the loop for the given process      
  }//end of cycle over processes
  
  
  
  //condition histograms
  for(auto var = variables.begin(); var != variables.end() ; var++){
    std::string vname = var -> VarName;

    leg[vname]->AddEntry(data[vname], "Data","pad1");
    
    data[vname]->SetFillColor(78);
    if (var -> logscale) data[vname]-> GetYaxis() -> SetRangeUser(0.1, (data[vname] -> GetMaximum())*7.);
    else  data[vname]-> GetYaxis() -> SetRangeUser(0.1, (data[vname] -> GetMaximum())*1.5);
    data[vname]->GetYaxis()->SetTitle("Number of events");
    data[vname]->SetMarkerColor(DataSample.color);
    data[vname]->SetMarkerStyle(21);
    data[vname]->GetXaxis() -> SetLabelSize(0.);
    data[vname]->GetXaxis() -> SetLabelOffset(100000.);
    
    signalHist[vname] -> SetLineColor(SignalSample.color);
    signalHist[vname] -> SetLineWidth(2.);

    for (uint process_i = 0; process_i < samples.size(); process_i++){
      std::string process = samples[process_i].Processname;
      std::pair<std::string,std::string> key(vname,process);

      hs[vname] -> Add(hist_per_process[key], "bar");
      hist_summed[vname] -> Add(hist_per_process[key]);
      hist_per_process[key] -> SetFillColor(samples.at(process_i).color);
      hist_per_process[key] -> SetLineColor(samples.at(process_i).color);
      hist_per_process[key] -> GetYaxis() -> SetRangeUser(0.1, (hist_per_process[key] -> GetMaximum())*1.5);
      hist_per_process[key] -> SetLineColor(kBlack);
      hist_per_process[key] -> SetLineWidth(1.);
      leg[vname]->AddEntry(hist_per_process[key], (samples.at(process_i).Processname).c_str(),"f");
    }
  }
          
  //cosmetics
  for(auto var = variables.begin(); var != variables.end() ; var++){
    std::string vname = var -> VarName;
    systematics.eval(&(*var), hist_summed[vname]);
    c1=  new TCanvas("c1","canvas",1200,800);
    pad1 = new TPad("pad1","This is pad1",0.0,0.25,0.8,1.0);
    pad2 = new TPad("pad2","This is pad2",0.0,0.02,0.8,0.25);
    
    c1 -> cd();
    leg[vname] -> AddEntry(signalHist[vname], SignalSample.Processname.c_str()); 	  	  	
    if(var->logscale)
      pad1 -> SetLogy();
    pad1->Draw();
    pad2->Draw();
    pad1 -> cd();
    
    if(withData)
      {	
	data[vname] -> Draw("E1");
	hs[vname]->Draw("hist SAME s(0,0)");
	hist_summed[vname] -> SetFillColor(kBlack);
	hist_summed[vname] -> SetFillStyle(3018);
	hist_summed[vname] -> Draw("E2 SAME");
	signalHist[vname] -> Draw("hist SAME");
	data[vname] -> Draw("E1 SAME");
	data[vname] -> GetXaxis() -> Draw("SAME");
      } 
    else { 
      hs[vname]->Draw("hist");
      signalHist[vname] -> Draw("HISTSAME");
    }
    c1 -> cd();
    
    TPaveText *pt = new TPaveText(0.15,0.83,0.35,0.93, "blNDC");
    pt -> SetFillStyle(0);
    pt -> SetBorderSize(0);
    if (channel == ELECTRON) pt -> AddText("Electron channel");
    else if (channel == MUON) pt -> AddText("Muon channel");
    else std::cerr << "no channel set..." << std::endl;
    pt -> Draw("SAME");
    
    pad1 -> SetTopMargin(0.07);
    pad1 -> SetBottomMargin(0.03);
    pad2 -> SetTopMargin(0.05);
    pad2 -> SetBottomMargin(0.42);
    pad1 -> SetRightMargin(0.05);
    pad2 -> SetRightMargin(0.05);
    pad2 -> cd();
    
    TH1D *data_dif = new TH1D((vname + "_dif").c_str(),( vname + "_dif").c_str(), Nbins,var->Range.low, var->Range.high);
    data_dif -> Sumw2();
    for (int iBin = 1; iBin <= hist_summed[vname] -> GetNbinsX(); ++iBin)
      {
	if (hist_summed[vname] -> GetBinContent(iBin) == 0.) data_dif -> SetBinContent(iBin,10000000.);
	else {
	  data_dif -> SetBinContent(iBin, ((data[vname] -> GetBinContent(iBin)) - (hist_summed[vname] -> GetBinContent(iBin)))/(hist_summed[vname] -> GetBinContent(iBin)));
	  data_dif -> SetBinError(iBin, (data[vname]-> GetBinError(iBin))/(hist_summed[vname] -> GetBinContent(iBin)));
	}
      }
    
    TH1D *data_dif_MCerr = new TH1D((vname + "_dif_MCerror").c_str(),( vname + "_dif_MCerror").c_str(), Nbins,var->Range.low, var->Range.high);
    data_dif_MCerr -> Sumw2();
    data_dif_MCerr -> SetFillColor(kGray);
    
    for (int iBin = 1; iBin <= Nbins ; ++iBin)
      {
	if (hist_summed[vname] -> GetBinContent(iBin) == 0.) {
	  data_dif_MCerr -> SetBinContent(iBin, 0.);
	  data_dif_MCerr -> SetBinError(iBin, 0.);
	}
	else {
	  data_dif_MCerr -> SetBinContent(iBin, 0.);
	  data_dif_MCerr -> SetBinError(iBin, (hist_summed[vname] -> GetBinError(iBin))/hist_summed[vname] -> GetBinContent(iBin) );
	}
      }
    
    TLine *line = new TLine(var->Range.low,0.,var->Range.high,0.);
    data_dif_MCerr -> SetMaximum(2.);
    data_dif_MCerr ->  SetMinimum(-2.);
    data_dif_MCerr -> GetYaxis() -> SetNdivisions(5);
    data_dif_MCerr -> GetYaxis() -> SetLabelSize(0.15);
    data_dif_MCerr -> GetXaxis() -> SetLabelSize(0.2);
    data_dif_MCerr -> GetYaxis()->SetTitle("#frac{Data - MC}{MC}");
    data_dif_MCerr -> GetXaxis()->SetTitle((var->Title).c_str());
    data_dif_MCerr -> GetXaxis()->SetTitleSize(0.2);
    data_dif_MCerr -> GetYaxis()->SetTitleOffset(0.3);
    data_dif_MCerr -> GetYaxis()->SetTitleSize(0.2);
    data_dif ->SetMarkerStyle(21);
    data_dif_MCerr -> Draw("E2");
    data_dif -> Draw("E1 SAME");
    line -> Draw("SAME");
    
    leg[vname] -> AddEntry(data_dif_MCerr, "Syst. unc", "f");
    c1 -> cd();
    leg[vname]->Draw("SAME");
    
    
    CMS_lumi( c1, 4, 0 );
    c1 -> SaveAs((OutPrefix_  + "png/"+ var->VarName + ".png").c_str());
    c1 -> Update();
    c1 -> Print((OutPrefix_  + "pdf/"+ var->VarName + ".pdf").c_str());
    c1 -> Clear();
    delete c1;
  }//end of cosmetics 
}