#include "Plotter.hpp"

void draw(std::string channel, std::string region, std::string tag)
{
	if (channel != "mu" && channel != "ele") {
		std::cerr << "Channel is not mu or ele. Stopped." << std::endl;
		exit(0);
	}
	if (region != "ttbar" && region != "WJets" && region != "TTBarEnrichedInclusive"  &&  region != "TTBarEnrichedBTagVeto") {
		std::cerr << "Control region should be ttbar or WJets.Stopped." << std::endl;
		exit(0);
	}
	vector <Var> variables;
	Var var;
	var.logscale = false;
	var.VarName = "Mjpruned";
	var.Title = "m_{jet pruned}";
	var.SetRange(30., 250.);
	variables.push_back(var);

	var.VarName = "jet_tau2tau1";
	var.Title = "#tau_{21}";
	var.SetRange(0., 1.1);
	variables.push_back(var);

	var.VarName = "MWW";
	var.Title = "m_{WV}";
	var.SetRange(400., 2500.);
	variables.push_back(var);

	var.VarName = "nPV";
	var.Title = "n_{PV}";
	var.SetRange(0., 30.);
	variables.push_back(var);
	
	var.VarName = "l_pt";
	var.Title = "p_{T, lepton}";
	var.SetRange(50., 800.);
	variables.push_back(var);

	var.VarName = "l_eta";
	var.Title = "#eta_{lepton}";
	var.SetRange(-2.5, 2.5);
	variables.push_back(var);

	var.VarName = "l_phi";
	var.Title = "#phi_{lepton}";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "jet_pt";
	var.Title = "p_{T, jet}";
	var.SetRange(200., 1000.);
	variables.push_back(var);

	var.VarName = "jet_eta";
	var.Title = "#eta_{jet}";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "jet_phi";
	var.Title = "#phi_{lepton}";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	var.VarName = "W_pt";
	var.Title = "p_{T, W}";
	var.SetRange(60., 1000.);
	variables.push_back(var);

	var.VarName = "W_eta";
	var.Title = "#eta_{W}";
	var.SetRange(-3., 3.);
	variables.push_back(var);

	var.VarName = "W_phi";
	var.Title = "#phi_{W}";
	var.SetRange(-3.5, 3.5);
	variables.push_back(var);

	


	var.VarName = "W_mass";
	var.Title = "m_{W lep}";
	var.SetRange(30., 800.);
	variables.push_back(var);

	var.VarName = "W_mt";
	var.Title = "m_{T, W lep}";
	var.SetRange(200., 1100.);
	variables.push_back(var);

	var.VarName = "pfMET";
	var.Title = "MET";
	var.SetRange(30., 900.);
	variables.push_back(var);

	var.VarName = "pfMETPhi";
	var.Title = "#phi(MET)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);

	var.VarName = "jet_mass_softdrop";
	var.Title = "m_{jet softdrop}";
	var.SetRange(0., 250.);
	variables.push_back(var);

	var.VarName = "njets";
	var.Title = "n_{jets}";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "nbtag";
	var.Title = "n_{btag}";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "nLooseEle";
	var.Title = "nLooseEle";
	var.SetRange(0., 2.);
	variables.push_back(var);

	var.VarName = "nLooseMu";
	var.Title = "nLooseMu";
	var.SetRange(0., 2.);
	variables.push_back(var);


	var.VarName = "NAK8jet";
	var.Title = "n_{fat jets}";
	var.SetRange(0., 6.);
	variables.push_back(var);

	var.VarName = "N_lep_W";
	var.Title = "N_{lep W}";
	var.SetRange(-1., 6.);
	variables.push_back(var);

	var.VarName = "charge_W_lep";
	var.Title = "charge(W lep)";
	var.SetRange(-2., 2.);
	variables.push_back(var);

	var.VarName = "deltaR_LeptonWJet";
	var.Title = "#DeltaR(Lep, WJet)";
	var.SetRange(0., 7.);
	variables.push_back(var);

	var.VarName = "deltaPhi_LeptonMet";
	var.Title = "#Delta#phi(Lep, MET)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);

	var.VarName = "deltaPhi_WJetMet";
	var.Title = "#Delta#phi(WJet, MET)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);

	var.VarName = "deltaPhi_WJetWlep";
	var.Title = "#Delta#phi(WJet, WLep)";
	var.SetRange(-3.2, 3.2);
	variables.push_back(var);

	Plotter p;

	if (channel == "mu")p = Plotter(MUON);
	else if (channel == "ele" )  p = Plotter(ELECTRON);
	else exit(0);
	vector <Sample> samples;
	p.SetVar(variables);
	p.SetNbins(30);

	
	string defaulCuts = "(jet_pt > 200. && jet_tau2tau1 < 0.6  && Mjpruned < 150. && Mjpruned > 40. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900.";
	if (channel == "ele") defaulCuts += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") defaulCuts += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}
	string addOnCutWjets = defaulCuts +  " * ( (Mjpruned < 65. || Mjpruned > 105. ) && nbtag == 0) ";
	string addOnCutTtbar = defaulCuts +  " * (nbtag > 0 )";

	string TTBarEnrichedInclusive = "jet_pt > 200.  &&  jet_tau2tau1 < 0.6  && Mjpruned < 200. && Mjpruned > 155. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900. ";
	if (channel == "ele") TTBarEnrichedInclusive += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") TTBarEnrichedInclusive += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}

	string TTBarEnrichedBTagVeto = "jet_pt > 200.  &&  jet_tau2tau1 < 0.6  && Mjpruned < 200. && Mjpruned > 155. && W_pt > 200.  && abs(deltaR_LeptonWJet) > pi/2. && abs(deltaPhi_WJetMet) > 2. && abs(deltaPhi_WJetWlep) > 2. && MWW > 900. && nbtag == 0 ";
	if (channel == "ele") TTBarEnrichedBTagVeto += " && l_pt > 50. && pfMET > 80. )"; 
	else if (channel == "mu") TTBarEnrichedBTagVeto += " && l_pt > 50. && pfMET > 40. )"; 
	else {
		std::cerr << "Invalid channel used, use ele or mu" << std::endl;
		exit(0);
	}

	std::string MCSelection,SignalSelection,DataSelection;

	if (region == "WJets"){
		MCSelection =  addOnCutWjets ;
		SignalSelection = "( " + addOnCutWjets + " )";
		DataSelection = addOnCutWjets;
	}
	else if(region == "ttbar"){
		MCSelection =  addOnCutTtbar;
		SignalSelection = "( " + addOnCutTtbar + " )";
		DataSelection = addOnCutTtbar;
	}
	else if(region == "TTBarEnrichedInclusive"){
		MCSelection =  TTBarEnrichedInclusive;
		SignalSelection = "( " + TTBarEnrichedInclusive + " )";
		DataSelection = TTBarEnrichedInclusive;
	}
	else if(region == "TTBarEnrichedBTagVeto"){
		MCSelection =  TTBarEnrichedBTagVeto;
		SignalSelection = "( " + TTBarEnrichedBTagVeto + " )";
		DataSelection = TTBarEnrichedBTagVeto;
	}
	else std::cout << "This should not happen ..." << std::endl;

		
	/*
	 * Colors
	 * kWhite =0,   kBlack =1,   kGray=920,  kRed   =632, kGreen =416, kBlue=600, 
	 * kYellow=400, kMagenta=616, kCyan=432, kOrange=800, kSpring=820, kTeal=840,
	 * kAzure =860, kViolet =880, kPink=900
	 */
	
	Sample s, dataSample, signalSample;
	
	string prefix = "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_31March2016_v3/";
	
	s.SetParameters("WW", MCSelection, kRed);
 	s.SetFileNames( prefix + "WW_"+ channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("WZ", MCSelection, kRed-7);
 	s.SetFileNames( prefix + "WZ_"+ channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("W+jets", MCSelection, kGreen);
 	s.SetFileNames(prefix + "WJets_Ht100To200_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht200To400_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht400To600_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht600To800_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht800To1200_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht1200To2500_" + channel + ".root");
 	s.SetFileNames(prefix + "WJets_Ht2500ToInf_" + channel + ".root");
 	s.weight = 1.16;
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("ttbar", MCSelection, kOrange);
 	s.SetFileNames(prefix + "ttbar_" + channel + ".root");
	samples.push_back(s);
	s.ReSet();

	s.SetParameters("Single Top", MCSelection, kBlue);
 	s.SetFileNames(prefix + "tW-ch-top_" + channel + ".root");
 	s.SetFileNames(prefix + "tW-ch-antitop_" + channel + ".root");
 	s.SetFileNames(prefix + "t-ch_" + channel + ".root");
 	s.SetFileNames(prefix + "s-ch_" + channel + ".root");
	samples.push_back(s);
	s.ReSet();

	dataSample.SetParameters("data", DataSelection, kBlack);
 	dataSample.SetFileNames(prefix + "data-RunD_" + channel + ".root");

 	signalSample.SetParameters("#splitline{madgraph EWDim6}{c_{WWW} = 12 TeV^{-2}}", SignalSelection, kRed);
 	signalSample.SetFileNames(prefix + "WW-aTGC_"+ channel + ".root");
	
	
	p.SetSamples(samples);
	p.DataSample = dataSample;
	p.SignalSample = signalSample;
	p.withData = true;
 	p.Plotting(("plots_25ns_" + channel + "_" + tag + "/").c_str());

}

/*
* to compile:
* g++ -o draw  draw.cpp --std=c++11 `root-config --cflags --libs` -O2 -I./ -lASImage
* and then you run like this:
* ./draw ele ttbar
* enjoy it!
*/

int main(int argc, char* argv[]){

	if (argc != 4) std::cerr << "You should use only 2 arguments ..." << std::endl; 
 	draw(string(argv[1]), string(argv[2]), string(argv[3]));
}


