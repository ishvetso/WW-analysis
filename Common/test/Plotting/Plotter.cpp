#include "Plotter.hpp"

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
  	for (uint file_i = 0; file_i < SignalSample.filenames.size(); ++file_i)
    { 
     	TFile file((SignalSample.filenames.at(file_i)).c_str(), "READ");
      	TTree * tree = (TTree*)file.Get("treeDumper/BasicTree");
      	TTreeFormula *signalSelection = new TTreeFormula("signalSelection",SignalSample.selection.c_str(),tree);//that should be without any weights!
      
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
          		if(signalSelection -> EvalInstance())signalHist[vname]->Fill(var->value());//check if the event passeds the selection, and if true fill the histogram
        	}
      	}
    }// end loop over signal files

     //condition histograms
  	for(auto var = variables.begin(); var != variables.end() ; var++){
	    std::string vname = var->VarName;
	    leg[vname]->AddEntry(data[vname], "Data","pad1");

	    data[vname]->SetFillColor(78);
	    data[vname]-> GetYaxis() -> SetRangeUser(0.1, (data[vname] -> GetMaximum())*7.);
	    data[vname]->GetYaxis()->SetTitle("Number of events");
	    data[vname]->SetMarkerColor(DataSample.color);
	    data[vname]->SetMarkerStyle(21);
	    data[vname]->GetXaxis() -> SetLabelSize(0.);
	    data[vname]->GetXaxis() -> SetLabelOffset(100000.);

	    signalHist[vname] -> SetLineColor(SignalSample.color);
	    signalHist[vname] -> SetLineWidth(2.);
  	}
	  

	//Monte carlo samples
	//beginning of cycle over processes
	for (uint process_i = 0; process_i < samples.size(); process_i++)
	{
		//create a buffer for the process		  	
	  	std::map<std::string,TH1D*> hist_per_process;
      	for(auto var = variables.begin(); var != variables.end() ; var++){
        		std::string vname = var-> VarName;
        		hist_per_process[vname] = new TH1D((samples.at(process_i).Processname + "_" + vname).c_str(),(samples.at(process_i).Processname + "_" + vname).c_str(), Nbins,var->Range.low, var->Range.high);
	    		hist_per_process[vname] -> Sumw2();
      	}
	    //loop over files for the given process
	    for (uint file_i = 0; file_i < (samples.at(process_i)).filenames.size(); ++file_i)
	    {
	      	TFile file((samples.at(process_i)).filenames.at(file_i).c_str(), "READ");
	      	std::cout << (samples.at(process_i)).filenames.at(file_i) << std::endl; 
      		TTree * tree = (TTree*)file.Get("BasicTree;2");
      		Double_t totEventWeight;
      		double genWeight, lumiWeight,PUWeight;
      		tree -> SetBranchAddress("totEventWeight", &totEventWeight);
      		TTreeFormula *MCSampleSelection = new TTreeFormula("MCSampleSelection",samples.at(process_i).selection.c_str(),tree);//that should be without any weights!

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
          		if(MCSampleSelection -> EvalInstance())hist_per_process[vname]->Fill(var->value(), totEventWeight);//check if the event passeds the selection, and if true fill the histogram
        		}
      		}
	    }// end of the loop for the given process

	    for(auto var = variables.begin(); var != variables.end() ; var++){
	    	std::string vname = var -> VarName;
	    	hs[vname] -> Add(hist_per_process[vname], "bar");
	    	hist_summed[vname] -> Add(hist_per_process[vname]);
	    	hist_per_process[vname] -> SetFillColor(samples.at(process_i).color);
	    	hist_per_process[vname] -> SetLineColor(samples.at(process_i).color);
		   	hist_per_process[vname] -> GetYaxis() -> SetRangeUser(0.1, (hist_per_process[vname] -> GetMaximum())*1.5);
		   	hist_per_process[vname] -> SetLineColor(kBlack);
		   	hist_per_process[vname] -> SetLineWidth(1.);
		   	leg[vname]->AddEntry(hist_per_process[vname], (samples.at(process_i).Processname).c_str(),"f");
	    }


	}//end of cycle over processes
	//
	//cosmetics
	for(auto var = variables.begin(); var != variables.end() ; var++){
		std::string vname = var -> VarName;
		Systematics(*var, hist_summed[vname]);
		c1=  new TCanvas("c1","canvas",1200,800);
	    pad1 = new TPad("pad1","This is pad1",0.0,0.25,0.8,1.0);
	    pad2 = new TPad("pad2","This is pad2",0.0,0.02,0.8,0.25);

	    c1 -> cd();
		leg[vname] -> AddEntry(signalHist[vname], SignalSample.Processname.c_str()); 	  	  	
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
	    pad2 -> SetBottomMargin(0.32);
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
    	c1 -> SaveAs((OutPrefix_ + var->VarName + ".png").c_str());
    	c1 -> Clear();
    	delete c1;

	    /*delete data;
	    delete data_dif_MCerr;
	    delete data_dif;
	    delete hist_summed;
	    delete hs;
	    delete signalHist;*/
	}//end of cosmetics 
}



void Plotter::Systematics(Var var, TH1D *& hist_nominal)
{
	//list of supported systematics
	std::vector<std::string> ListOfSystematics;
	ListOfSystematics.push_back("JEC");
	ListOfSystematics.push_back("LeptonEn");
	ListOfSystematics.push_back("LeptonRes");
	ListOfSystematics.push_back("UnclEn");
	std::map <std::string, std::vector<std::string>> VariablesAffected;
	std::vector<double> totalErrorQuadraticErrors(Nbins, 0.);
	
	//add statistical uncertainty from MC first
	for (unsigned int iBin = 0; iBin < Nbins; iBin++){
		totalErrorQuadraticErrors[iBin] = std::pow(hist_nominal -> GetBinError(iBin), 2);
	}
	
	//JEC
	std::vector<std::string> VarsJEC;
	VarsJEC.push_back("W_pt");
	VarsJEC.push_back("W_mass");
	VarsJEC.push_back("W_mt");
	VarsJEC.push_back("pfMETPhi");
	VarsJEC.push_back("pfMET");
	VarsJEC.push_back("jet_pt");
	VarsJEC.push_back("Mjpruned");
	VarsJEC.push_back("jet_mass_softdrop");
	VarsJEC.push_back("jet_mass");
	VarsJEC.push_back("m_lvj");
	VarsJEC.push_back("deltaPhi_LeptonMet");
	VarsJEC.push_back("deltaPhi_WJetMet");
	VarsJEC.push_back("deltaPhi_WJetWlep");
	//En (lepton energy scale)
	std::vector<std::string> VarsLeptonEn;
	VarsLeptonEn.push_back("l_pt");
	VarsLeptonEn.push_back("W_pt");
	VarsLeptonEn.push_back("W_mass");
	VarsLeptonEn.push_back("W_mt");
	VarsLeptonEn.push_back("pfMET");
	VarsLeptonEn.push_back("pfMETPhi");
	VarsLeptonEn.push_back("deltaPhi_LeptonMet");
	VarsLeptonEn.push_back("deltaPhi_WJetMet");
	VarsLeptonEn.push_back("deltaPhi_WJetWlep");
	VarsLeptonEn.push_back("m_lvj");
	//lepton resolution
	std::vector<std::string> VarsLeptonRes;
	VarsLeptonRes.push_back("l_pt");
	VarsLeptonRes.push_back("W_pt");
	VarsLeptonRes.push_back("W_mass");
	VarsLeptonRes.push_back("W_mt");
	VarsLeptonRes.push_back("m_lvj");

	//MET
	std::vector<std::string> VarsUnclEn;
	VarsUnclEn.push_back("W_pt");
	VarsUnclEn.push_back("W_mass");
	VarsUnclEn.push_back("W_mt");
	VarsUnclEn.push_back("pfMET");
	VarsUnclEn.push_back("pfMETPhi");
	VarsUnclEn.push_back("deltaPhi_LeptonMet");
	VarsUnclEn.push_back("deltaPhi_WJetMet");
	VarsUnclEn.push_back("deltaPhi_WJetWlep");
	VarsUnclEn.push_back("m_lvj");

	VariablesAffected.insert(std::pair<std::string, std::vector<std::string>>("JEC", VarsJEC));
	VariablesAffected.insert(std::pair<std::string, std::vector<std::string>>("LeptonEn", VarsLeptonEn));
	VariablesAffected.insert(std::pair<std::string, std::vector<std::string>>("LeptonRes", VarsLeptonRes));
	VariablesAffected.insert(std::pair<std::string, std::vector<std::string>>("UnclEn", VarsUnclEn));

	bool isAffected = false;
	

	std::map<std::string,TH1D*> hist_SystUp;
	std::map<std::string,TH1D*> hist_SystDown;

	//create histograms for each of systematics and defined if the variable should be varied (is affected)
	for (unsigned int iSyst = 0; iSyst < ListOfSystematics.size(); iSyst ++)
    {
    	if (std::find(VariablesAffected[ListOfSystematics.at(iSyst)].begin(), VariablesAffected[ListOfSystematics.at(iSyst)].end(), var.VarName) != VariablesAffected[ListOfSystematics.at(iSyst)].end()) isAffected = true;
		hist_SystUp[ListOfSystematics.at(iSyst)] = new TH1D((var.VarName + "_" + ListOfSystematics.at(iSyst) + "Up").c_str(),(var.VarName + "_" +  ListOfSystematics.at(iSyst) + "Up" ).c_str(), Nbins, var.Range.low, var.Range.high);
		hist_SystDown[ListOfSystematics.at(iSyst)] = new TH1D((var.VarName + "_" + ListOfSystematics.at(iSyst) + "Down").c_str(),(var.VarName + "_"+ ListOfSystematics.at(iSyst) + "Down" ).c_str(), Nbins, var.Range.low, var.Range.high);
		hist_SystUp[ListOfSystematics.at(iSyst)] -> Sumw2();
		hist_SystDown[ListOfSystematics.at(iSyst)] -> Sumw2();
	}

	//beginning of cycle over files corresponding to each process
	for (uint process_i = 0; process_i < samples.size(); process_i++)
	{		
		//prepare string with selection for varied systematics 
		std::map<std::string, std::string> selection_Up, selection_Down;
		for (unsigned int iSyst = 0; iSyst < ListOfSystematics.size(); iSyst ++)
		{
			selection_Up[ListOfSystematics.at(iSyst)] = samples.at(process_i).selection;
	    	selection_Down[ListOfSystematics.at(iSyst)] = samples.at(process_i).selection;
	    	//split the string of selection, this is needed because some of variables start with the same characters, thus we split the string to be sure we get correct variable
       		std::vector<std::string> splittedSelectionUp;	
       		boost::split(splittedSelectionUp,selection_Up[ListOfSystematics.at(iSyst)],boost::is_any_of(" *+=-:/)(|&<>0123456789."));
        	//erase duplicates from the string
        	std::sort(splittedSelectionUp.begin(), splittedSelectionUp.end());
        	splittedSelectionUp.erase(std::unique(splittedSelectionUp.begin(), splittedSelectionUp.end()), splittedSelectionUp.end());
   		    for (unsigned int iS = 0; iS < splittedSelectionUp.size(); iS ++)
		    {
	      		if (splittedSelectionUp[iS].empty()) continue;
	      		bool found = false;
	      		for (unsigned int iVarSyst = 0 ; iVarSyst < VariablesAffected[ListOfSystematics.at(iSyst)].size(); iVarSyst ++){
	      		 	 found = ( VariablesAffected[ListOfSystematics.at(iSyst)].at(iVarSyst) == splittedSelectionUp[iS] );
	      			if (found) {
	      				boost::replace_all(selection_Up[ListOfSystematics.at(iSyst)],    VariablesAffected.at(ListOfSystematics.at(iSyst)).at(iVarSyst),  VariablesAffected.at(ListOfSystematics.at(iSyst)).at(iVarSyst) + "_" + ListOfSystematics.at(iSyst) + "Up");
	      				boost::replace_all(selection_Down[ListOfSystematics.at(iSyst)],  VariablesAffected.at(ListOfSystematics.at(iSyst)).at(iVarSyst),  VariablesAffected.at(ListOfSystematics.at(iSyst)).at(iVarSyst) + "_"+ ListOfSystematics.at(iSyst) + "Down");
	      				continue;
	      			}
	       		}
		    }

		}    
	    

		for (uint file_i = 0; file_i < (samples.at(process_i)).filenames.size(); ++file_i)
		{
	    	TFile file((samples.at(process_i).filenames.at(file_i)).c_str(), "READ");
      		TTree * tree = (TTree*)file.Get("BasicTree;2");
      		std::map<std::string, TTreeFormula*> selectionUpInFormula, selectionDownInFormula;
      		Double_t totEventWeight;
      		tree -> SetBranchAddress("totEventWeight", &totEventWeight);
      		var.Initialize(tree);
      		for (uint iSyst =0; iSyst < ListOfSystematics.size(); iSyst++)
      		{
      			selectionUpInFormula[ListOfSystematics[iSyst]] = new TTreeFormula(("selection_Up" + ListOfSystematics[iSyst]).c_str(), selection_Up[ListOfSystematics[iSyst]].c_str(), tree);
      			selectionDownInFormula[ListOfSystematics[iSyst]] = new TTreeFormula(("selection_Down" + ListOfSystematics[iSyst]).c_str(), selection_Down[ListOfSystematics[iSyst]].c_str(), tree);
      		}
   		    for (unsigned int iEntry = 0; iEntry < tree->GetEntriesFast(); iEntry++)
		    {
		    	tree -> GetEntry(iEntry);
		    	for (uint iSyst =0; iSyst < ListOfSystematics.size(); iSyst++)
		    	{
		    		if(selectionUpInFormula[ListOfSystematics.at(iSyst)] -> EvalInstance())hist_SystUp[ListOfSystematics.at(iSyst)] -> Fill(var.value(), totEventWeight);
		    		if(selectionDownInFormula[ListOfSystematics.at(iSyst)] -> EvalInstance())hist_SystDown[ListOfSystematics.at(iSyst)] -> Fill(var.value(), totEventWeight);
		    	}	
	    	}
	    } 

	}//end of the cycle over processes
	  	    
	//now sum up the differences quadratically
	for (unsigned int iBin = 1; iBin <= hist_nominal -> GetNbinsX(); iBin ++){
		for (uint iSyst = 0; iSyst < ListOfSystematics.size(); iSyst++)
		{
			double errorUpQuadratic = pow(std::abs((hist_SystUp[ListOfSystematics.at(iSyst)] -> GetBinContent(iBin)) - (hist_nominal -> GetBinContent(iBin))), 2);
		 	double errorDownQuadratic = pow(std::abs((hist_SystDown[ListOfSystematics.at(iSyst)] -> GetBinContent(iBin)) - (hist_nominal -> GetBinContent(iBin))), 2);
		 	double errorQuadratic = std::max(errorUpQuadratic, errorDownQuadratic);
		 	totalErrorQuadraticErrors.at(iBin-1) += errorQuadratic;

		}
	}

  	int iBin = 1; 
    //now set the uncertainty  	
  	for (double & error : totalErrorQuadraticErrors) {
  		error = std::sqrt(error);
  		hist_nominal -> SetBinError(iBin, error);
  		iBin++;
  	}


}