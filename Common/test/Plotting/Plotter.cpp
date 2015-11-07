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
	   // data_dif -> Sumw2();
	    for (int iBin = 1; iBin <hist_summed -> GetNbinsX(); ++iBin)
	    {
			if (hist_summed -> GetBinContent(iBin) == 0.) data_dif -> SetBinContent(iBin,10000000.);
			else data_dif -> SetBinContent(iBin, ((data -> GetBinContent(iBin)) - (hist_summed -> GetBinContent(iBin)))/(hist_summed -> GetBinContent(iBin)));
	    }

	    TLine *line = new TLine(variables.at(var_i).Range.low,0.,variables.at(var_i).Range.high,0.);
	    data_dif -> SetMaximum(2.);
	    data_dif ->  SetMinimum(-2.);
	    data_dif -> GetYaxis() -> SetNdivisions(5);
	    data_dif -> GetYaxis() -> SetLabelSize(0.15);
	    data_dif -> GetXaxis() -> SetLabelSize(0.2);
	    data_dif -> GetYaxis()->SetTitle("#frac{Data - MC}{MC}");
	    data_dif -> GetXaxis()->SetTitle((variables.at(var_i)).Title.c_str());
	    data_dif -> GetXaxis()->SetTitleSize(0.2);
	    data_dif -> GetYaxis()->SetTitleOffset(0.3);
	    data_dif -> GetYaxis()->SetTitleSize(0.2);
	    data_dif ->SetMarkerStyle(21);
	    data_dif -> Draw("E1");
	    line -> Draw("SAME");
	   
	    CMS_lumi( c1, 4, 0 );
	    c1 -> SaveAs((OutPrefix_ + variables.at(var_i).VarName + ".png").c_str());
	    c1 -> Clear();
	}
	//end of cycle over variables
	
}