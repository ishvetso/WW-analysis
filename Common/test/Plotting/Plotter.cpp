#include "Plotter.hpp"

Plotter::Plotter()
{
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
	TCanvas *c1=  new TCanvas("c1","canvas",1200,800);
	
	
	
	//beginning of cycle over variables
	for (uint var_i = 0; var_i < variables.size(); ++ var_i )
	{	
	  TPad *pad1 = new TPad("pad1","This is pad1",0.0,0.25,0.8,1.0);
	  TPad *pad2 = new TPad("pad2","This is pad2",0.0,0.02,0.8,0.25);

	  
	  THStack *hs = new THStack("hs",(";"+ variables.at(var_i).VarName +";Number of events").c_str());
	  c1 -> cd();
	  TLegend *leg = new TLegend(0.8,0.7,0.98,0.93);
	  leg ->  SetFillColor(kWhite);
	  TH1D *hist_summed = new TH1D(); // sum of all processes for a given variable
	  *hist_summed = TH1D((variables.at(var_i).VarName + "summed").c_str(),( variables.at(var_i).VarName + "summed").c_str(), Nbins, variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	  hist_summed -> Sumw2();
	    
	//beginning of cycle over processes
	  for (uint process_i = 0; process_i < samples.size(); process_i++)
	  {
	     TH1D *hist = new TH1D(((samples.at(process_i)).Processname + variables.at(var_i).VarName).c_str(),((samples.at(process_i)).Processname + variables.at(var_i).VarName).c_str(), Nbins, variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	     hist -> Sumw2();

	    
	     //beginning of cycle over files corresponding to each process
	    for (uint file_i = 0; file_i < (samples.at(process_i)).filenames.size(); ++file_i)
	    {
	      cout << (samples.at(process_i)).filenames.at(file_i) << endl;
	      TFile file(((samples.at(process_i)).filenames.at(file_i)).c_str(), "READ");
	      TTree * tree = (TTree*)file.Get("BasicTree");
	      TH1D *temp = new TH1D((((samples.at(process_i)).Processname)+ "_temp").c_str(),((samples.at(process_i)).Processname).c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	      temp -> Sumw2();
	      tree ->Project(((samples.at(process_i)).Processname + "_temp").c_str(),(variables.at(var_i).VarName).c_str(), (samples.at(process_i).selection).c_str());
	      hist -> Add(temp);	      
	    }
	    //end of cycle over files corresponding to each process
	    hist -> SetFillStyle(2001);
	    hist -> SetFillColor(samples.at(process_i).color);
	    hist -> SetLineColor(samples.at(process_i).color);
	   	hist -> GetYaxis() -> SetRangeUser(0.1, (hist -> GetMaximum())*1.5);
	   	hist->GetXaxis() -> SetLabelSize(0.);
	    hs -> Add(hist, "bar");
	    hist_summed -> Add(hist);
	    
	    
	    leg->AddEntry(hist, (samples.at(process_i).Processname).c_str(),"f");
	   

	  }
	 
	  Systematics(variables.at(var_i), hist_summed);
	  
	  //hist_summed -> SetError(&errors[0]);
	  //end of cycle over processes
	  TH1D *data = new TH1D((DataSample.Processname + variables.at(var_i).VarName + "_data").c_str(),(DataSample.Processname + variables.at(var_i).VarName + "_data").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	  data -> Sumw2();
	      
		for (uint file_i = 0; file_i < DataSample.filenames.size(); ++file_i)
		{
		    TFile file((DataSample.filenames.at(file_i)).c_str(), "READ");
		    TTree * tree = (TTree*)file.Get("treeDumper/BasicTree");
		    TH1D *temp = new TH1D(((DataSample.Processname)+ "_temp").c_str(),((DataSample.Processname)+ "_temp").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
		    tree ->Project(((DataSample.Processname)+ "_temp").c_str(),(variables.at(var_i).VarName).c_str(), (DataSample.selection).c_str());
		    data -> Add(temp);
		}

		leg->AddEntry(data, "Data","pad1");
	      
		data->SetFillColor(78);
		data->SetFillStyle(3004);
		data -> GetYaxis() -> SetRangeUser(0.1, (data -> GetMaximum())*7.);
		//ata->GetXaxis()->SetTitle((variables.at(var_i)).VarName.c_str());
		data->GetYaxis()->SetTitle("Number of events");
		data->SetMarkerColor(DataSample.color);
		data->SetMarkerStyle(21);

	  	
	  	pad1 -> SetLogy();
	    pad1->Draw();
	    pad2->Draw();
	    pad1 -> cd();
		if(withData)
		{	
			data -> Draw("E1");
	    	hs->Draw("hist SAME");
	    	hist_summed -> SetFillColor(kBlack);
  			hist_summed -> SetFillStyle(3018);
	    	hist_summed -> Draw("E2 SAME");
	    	data -> Draw("E1 SAME");
	    	data -> GetXaxis() -> Draw("SAME");
		} 
		else hs->Draw("hist");
	    c1 -> cd();
	    leg->Draw("SAME");
        
        pad1 -> SetTopMargin(0.07);
        pad1 -> SetBottomMargin(0.);
        pad2 -> SetTopMargin(0.0);
        pad2 -> SetBottomMargin(0.32);
	    pad2 -> cd();
	  

	    TH1D *data_dif = new TH1D((variables.at(var_i).VarName + "_dif").c_str(),( variables.at(var_i).VarName + "_dif").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	    data_dif -> Sumw2();
	    for (int iBin = 1; iBin <= hist_summed -> GetNbinsX(); ++iBin)
	    {
			if (hist_summed -> GetBinContent(iBin) == 0.) data_dif -> SetBinContent(iBin,10000000.);
			else {
				data_dif -> SetBinContent(iBin, ((data -> GetBinContent(iBin)) - (hist_summed -> GetBinContent(iBin)))/(hist_summed -> GetBinContent(iBin)));
				data_dif -> SetBinError(iBin, (data-> GetBinError(iBin))/(hist_summed -> GetBinContent(iBin)));
			}
	    }
	    
	    TH1D *data_dif_MCerr = new TH1D((variables.at(var_i).VarName + "_dif_MCerror").c_str(),( variables.at(var_i).VarName + "_dif_MCerror").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	    data_dif_MCerr -> Sumw2();
	    data_dif_MCerr -> SetFillColor(kGray);

	    for (int iBin = 1; iBin <= Nbins ; ++iBin)
	    {
	    	if (hist_summed -> GetBinContent(iBin) == 0.) {
	    		data_dif_MCerr -> SetBinContent(iBin, 0.);
	    		data_dif_MCerr -> SetBinError(iBin, 0.);
	    	}
	    	else {
	    		data_dif_MCerr -> SetBinContent(iBin, 0.);
	    		data_dif_MCerr -> SetBinError(iBin, (hist_summed -> GetBinError(iBin))/hist_summed -> GetBinContent(iBin) );
	    	}
	    }

	    TLine *line = new TLine(variables.at(var_i).Range.low,0.,variables.at(var_i).Range.high,0.);
	    data_dif_MCerr -> SetMaximum(2.);
	    data_dif_MCerr ->  SetMinimum(-2.);
	    data_dif_MCerr -> GetYaxis() -> SetNdivisions(5);
	    data_dif_MCerr -> GetYaxis() -> SetLabelSize(0.15);
	    data_dif_MCerr -> GetXaxis() -> SetLabelSize(0.2);
	    data_dif_MCerr -> GetYaxis()->SetTitle("#frac{Data - MC}{MC}");
	    data_dif_MCerr -> GetXaxis()->SetTitle((variables.at(var_i)).Title.c_str());
	    data_dif_MCerr -> GetXaxis()->SetTitleSize(0.2);
	    data_dif_MCerr -> GetYaxis()->SetTitleOffset(0.3);
	    data_dif_MCerr -> GetYaxis()->SetTitleSize(0.2);
	    data_dif ->SetMarkerStyle(21);
	    data_dif_MCerr -> Draw("E2");
	    data_dif -> Draw("E1 SAME");
	    line -> Draw("SAME");
	   
	    CMS_lumi( c1, 4, 0 );
	    c1 -> SaveAs((OutPrefix_ + variables.at(var_i).VarName + ".png").c_str());
	    c1 -> Clear();
	}
	//end of cycle over variables
	
}


void Plotter::Systematics(Var var, TH1D * hist_nominal)
{
	std::vector<std::string> ListOfSystematics;
	ListOfSystematics.push_back("JEC");
	ListOfSystematics.push_back("En");
	std::map <std::string, std::vector<std::string>> VariablesAffected;

	//JEC
	std::vector<std::string> VarsJEC;
	VarsJEC.push_back("jet_pt");
	VarsJEC.push_back("jet_mass_pruned");
	VarsJEC.push_back("jet_mass_softdrop");
	VarsJEC.push_back("jet_mass");
	VarsJEC.push_back("m_lvj");
	VarsJEC.push_back("pfMETPhi");
	VarsJEC.push_back("pfMET");
	//En (lepton energy scale)
	std::vector<std::string> VarsEn;
	VarsEn.push_back("l_pt");
	

	VariablesAffected.insert(std::pair<std::string, std::vector<std::string>>("JEC", VarsJEC));


	bool isAffected = false;
	if (std::find(VariablesAffected["JEC"].begin(), VariablesAffected["JEC"].end(), var.VarName) != VariablesAffected["JEC"].end()) isAffected = true;


	TH1D *hist_JECUp = new TH1D((var.VarName + "_JECUp").c_str(),(var.VarName + "_JECUp" ).c_str(), Nbins, var.Range.low, var.Range.high);
	TH1D *hist_JECDown = new TH1D((var.VarName + "_JECDown").c_str(),(var.VarName + "_JECDown" ).c_str(), Nbins, var.Range.low, var.Range.high);
	hist_JECUp -> Sumw2();
	hist_JECDown -> Sumw2();

	 for (uint process_i = 0; process_i < samples.size(); process_i++)
	  {		
	    //beginning of cycle over files corresponding to each process
	    for (uint file_i = 0; file_i < (samples.at(process_i)).filenames.size(); ++file_i)
	    {
	      std::string selection_Up = samples.at(process_i).selection;
	      std::string selection_Down = samples.at(process_i).selection;
	      //split the string of selection, this is needed because some of variables start with the same characters, thus we split the string to be sure we get correct variable
	      std::vector<std::string> splittedSelectionUp;	
	      boost::split(splittedSelectionUp,selection_Up,boost::is_any_of(" *+=-:/)(|&<>0123456789."));
	      //erase duplicates from the string
	      std::sort(splittedSelectionUp.begin(), splittedSelectionUp.end());
	      splittedSelectionUp.erase(std::unique(splittedSelectionUp.begin(), splittedSelectionUp.end()), splittedSelectionUp.end());
	      std::cout << selection_Up << std::endl;
	      std::cout << selection_Down << std::endl;
	      for (unsigned int iS = 0; iS < splittedSelectionUp.size(); iS ++){
	      		if (splittedSelectionUp[iS].empty()) continue;
	      		bool found = false;
	      		for (unsigned int iVarSyst = 0 ; iVarSyst < VariablesAffected["JEC"].size(); iVarSyst ++){
	      		 	 found = ( VariablesAffected["JEC"].at(iVarSyst) == splittedSelectionUp[iS] );
	      			if (found) {
	      				boost::replace_all(selection_Up,    VariablesAffected["JEC"].at(iVarSyst),  VariablesAffected["JEC"].at(iVarSyst) + "_JECUp");
	      				boost::replace_all(selection_Down,  VariablesAffected["JEC"].at(iVarSyst),  VariablesAffected["JEC"].at(iVarSyst) + "_JECDown");
	      				continue;
	      			}
	       		}
	      	}
	      
	      		
	      std::cout << selection_Up << std::endl;
	      std::cout << selection_Down << std::endl;
	      TFile file(((samples.at(process_i)).filenames.at(file_i)).c_str(), "READ");
	      TTree * tree = (TTree*)file.Get("BasicTree");
	      TH1D *tempUp = new TH1D((((samples.at(process_i)).Processname)+ "_tempUp").c_str(),((samples.at(process_i)).Processname + "_tempUp").c_str(), Nbins,var.Range.low, var.Range.high);
	      TH1D *tempDown = new TH1D((((samples.at(process_i)).Processname)+ "_tempDown").c_str(),((samples.at(process_i)).Processname + "_tempDown").c_str(), Nbins,var.Range.low, var.Range.high);
	      tempUp -> Sumw2();
	      tempDown -> Sumw2();
	      if (isAffected){
	      	tree ->Project(((samples.at(process_i)).Processname + "_tempUp").c_str(), ( var.VarName  + "_JECUp").c_str(), selection_Up.c_str() );
	      	tree ->Project(((samples.at(process_i)).Processname + "_tempDown").c_str(), ( var.VarName  + "_JECDown").c_str(), selection_Down.c_str()	 );
	  	  }
	  	  else {
	  	  	tree ->Project(((samples.at(process_i)).Processname + "_tempUp").c_str(), ( var.VarName ).c_str(),  selection_Up.c_str() );
	      	tree ->Project(((samples.at(process_i)).Processname + "_tempDown").c_str(), ( var.VarName ).c_str(),  selection_Down.c_str() );
	  	  }
	      hist_JECUp -> Add(tempUp);	     
	      hist_JECDown -> Add(tempDown);	      
	    }  

	  }

	 for (unsigned int iBin = 1; iBin <= hist_nominal -> GetNbinsX(); iBin ++){
	 	double errorUp = std::abs((hist_JECUp -> GetBinContent(iBin)) - (hist_nominal -> GetBinContent(iBin)));
	 	double errorDown = std::abs((hist_JECDown -> GetBinContent(iBin)) - (hist_nominal -> GetBinContent(iBin)));
	 	double error = std::max(errorUp, errorDown);
	 	hist_nominal -> SetBinError(iBin, error);
	 	std::cout << iBin << " " << error << std::endl;

	 }

}