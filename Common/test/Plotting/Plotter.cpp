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
	  TPad *pad1 = new TPad("pad1","This is pad1",0.0,0.0,0.8,1.);
	  //TPad *pad2 = new TPad("pad2","This is pad2",0.0,0.02,0.8,0.07);
	  
	  THStack *hs = new THStack("hs",(";"+ variables.at(var_i).VarName +";Number of events").c_str());
	  c1 -> cd();
	  TLegend *leg = new TLegend(0.8,0.93,0.98,0.8);
	  leg ->  SetFillColor(kWhite);
	  TH1D *hist_summed = new TH1D();
	//beginning of cycle over processes
	  for (uint process_i = 0; process_i < samples.size(); process_i++)
	  {
	     TH1D *hist = new TH1D(((samples.at(process_i)).Processname + variables.at(var_i).VarName).c_str(),((samples.at(process_i)).Processname + variables.at(var_i).VarName).c_str(), Nbins, variables.at(var_i).Range.low, variables.at(var_i).Range.high);

	    
	     *hist_summed = TH1D(((samples.at(process_i)).Processname + variables.at(var_i).VarName + "summed").c_str(),((samples.at(process_i)).Processname + variables.at(var_i).VarName + "summed").c_str(), Nbins, variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	    //beginning of cycle over files corresponding to each process
	    for (uint file_i = 0; file_i < (samples.at(process_i)).filenames.size(); ++file_i)
	    {
	      cout << (samples.at(process_i)).filenames.at(file_i) << endl;
	      TFile file(((samples.at(process_i)).filenames.at(file_i)).c_str(), "READ");
	      TTree * tree = (TTree*)file.Get("BasicTree");
	      TH1D *temp = new TH1D((((samples.at(process_i)).Processname)+ "_temp").c_str(),((samples.at(process_i)).Processname).c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	      tree ->Project(((samples.at(process_i)).Processname + "_temp").c_str(),(variables.at(var_i).VarName).c_str(), (samples.at(process_i).selection).c_str());
	      hist -> Add(temp);	      
	    }
	    //end of cycle over files corresponding to each process
	    hist -> SetFillStyle(2001);
	    hist -> SetFillColor(samples.at(process_i).color);
	    hist -> SetLineColor(samples.at(process_i).color);
	   	hist -> GetYaxis() -> SetRangeUser(0.1, (hist -> GetMaximum())*1.5);
	    hs -> Add(hist, "bar");
	    hist_summed -> Add(hist);
	    
	    
	    leg->AddEntry(hist, (samples.at(process_i).Processname).c_str(),"f");
	   

	  }
	  //end of cycle over processes
	  TH1D *data = new TH1D((DataSample.Processname + variables.at(var_i).VarName + "_data").c_str(),(DataSample.Processname + variables.at(var_i).VarName + "_data").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
	      
		for (uint file_i = 0; file_i < DataSample.filenames.size(); ++file_i)
		{
		    TFile file((DataSample.filenames.at(file_i)).c_str(), "READ");
		    TTree * tree = (TTree*)file.Get("treeDumper/BasicTree");
		    TH1D *temp = new TH1D(((DataSample.Processname)+ "_temp").c_str(),((DataSample.Processname)+ "_temp").c_str(), Nbins,variables.at(var_i).Range.low, variables.at(var_i).Range.high);
		    tree ->Project(((DataSample.Processname)+ "_temp").c_str(),(variables.at(var_i).VarName).c_str(), (DataSample.selection).c_str());
		    data -> Add(temp);
		}
	      
		data->SetFillColor(78);
		data->SetFillStyle(3004);
		data -> GetYaxis() -> SetRangeUser(0.1, (data -> GetMaximum())*7.);
		data->GetXaxis()->SetTitle((variables.at(var_i)).VarName.c_str());
		data->GetYaxis()->SetTitle("Number of events");
		data->SetMarkerColor(DataSample.color);
		data->SetMarkerStyle(21);

	  	
	  	pad1 -> SetLogy();
	    pad1->Draw();
	    //pad2->Draw();
	    pad1 -> cd();
		if(withData)
		{	
			data -> Draw("E1");
	    	hs->Draw("hist SAME");
	    	data -> Draw("E1 SAME");
		} 
		else hs->Draw("hist");
	    c1 -> cd();
	    leg->Draw();
           //  hs->Draw(); I'm currently not sure, may be this line is needed for data, check when using data
	   /* pad2 -> cd();

	    for (int iBin = 1; iBin <hist_summed -> GetNbinsX(); ++iBin)
	    {
			hist_summed -> SetBinContent(iBin, ((data -> GetBinContent(iBin)) - (hist_summed -> GetBinContent(iBin)))/(hist_summed -> GetBinContent(iBin)));
	    }
	    hist_summed -> Draw("hist SAME");*/
	   
	    CMS_lumi( c1, 4, 0 );
	    c1 -> SaveAs((OutPrefix_ + variables.at(var_i).VarName + ".png").c_str());
	    c1 -> Clear();
	}
	//end of cycle over variables
	
}