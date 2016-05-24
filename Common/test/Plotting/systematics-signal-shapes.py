#!/usr/bin/python 
from ROOT import  *
from array import array
from optparse import OptionParser
import math as math
import random
import os
import CMS_lumi, tdrstyle
from array import *
import math
import pandas

#change this!
#gSystem.Load('%s/lib/slc6_amd64_gcc481/libHiggsAnalysisCombinedLimit.so'%os.environ['CMSSW_BASE'])

#from ROOT import RooErfExpPdf, RooAlpha, RooAlpha4ErfPowPdf, RooAlpha4ErfPow2Pdf, RooAlpha4ErfPowExpPdf, RooPowPdf, RooPow2Pdf, RooErfPowExpPdf, RooErfPowPdf, RooErfPow2Pdf, RooQCDPdf, RooUser1Pdf, RooBWRunPdf, RooAnaExpNPdf, RooExpNPdf, RooAlpha4ExpNPdf, RooExpTailPdf, RooAlpha4ExpTailPdf, Roo2ExpPdf, RooAlpha42ExpPdf

POI	=	['cwww','ccw','cb']
par_max = {'cwww' : 12, 'ccw' : 20, 'cb' : 60}#atgc points

gStyle.SetOptStat(0)
gStyle.SetOptTitle(0)

def SetSystematicsFromFile(file_, histNominal_, ListOfSystematics_):
	binErrors = []
	binErrorsSquaredAdd = []
	histName = histNominal_.GetName()
	for iBin in range(1, histNominal_.GetNbinsX()):
		binErrors.append(histNominal_.GetBinError(iBin))
		binErrorsSquaredAdd.append(0.)
	for iSyst in ListOfSystematics_ :
		histUp = file_.Get(histName + "_" + iSyst + "Up")
		histDown = file_.Get(histName + "_" + iSyst + "Down")
		for iBin in range(1, histUp.GetNbinsX()):
			Up = histUp.GetBinContent(iBin) - histNominal_.GetBinContent(iBin)
			Down = histDown.GetBinContent(iBin) - histNominal_.GetBinContent(iBin)
			maxValue = max(abs(Up), abs(Down))
			binErrorsSquaredAdd[iBin-1] += pow(maxValue,2)
	for iBin in range(1, histNominal_.GetNbinsX()):
		histNominal_.SetBinError(iBin, sqrt( binErrorsSquaredAdd[iBin-1]))



def getSignalParmeters(cat, SMhist, pos_hists, neg_hists, ch = 'el',binlo=900,binhi=3500):

	if cat == 'WW':
		sigreg 	= 'lo'
		mj_lo	= 65.
		mj_hi	= 85.
	elif cat == 'WZ':
		sigreg	= 'hi'
		mj_lo	= 85.
		mj_hi	= 105
	else:
		raise RuntimeError('cateogry not supported!')

	nbins4fit	= 30
	WS		= RooWorkspace("WS")

	#prepare variables, parameters and temporary workspace
	wtmp		= RooWorkspace('wtmp')
	a1		= RooRealVar('a_SM_%s_%s'%(cat,ch),'a_SM_%s_%s'%(cat,ch),-0.1,-2,0)
	cwww		= RooRealVar('cwww','cwww',0,-120,120);
	ccw		= RooRealVar('ccw','ccw',0,-200,200);
	cb		= RooRealVar('cb','cb',0,-600,600);
	cwww.setConstant(kTRUE);
	ccw.setConstant(kTRUE);
	cb.setConstant(kTRUE);
	getattr(wtmp,'import')(cwww);
	getattr(wtmp,'import')(ccw);
	getattr(wtmp,'import')(cb);

	#make and fill SM histogram, SM fit
	
	#SMhist		= TH1F('SMhist','SMhist',nbins4fit,binlo,binhi)
	#SMhist.Sumw2(kTRUE)
	
	##read workspace containing background pdfs
	#fileInWs	= TFile.Open('Input/wwlvj_%s_HPW_workspace.root'%ch[:2])
	rrv_mass_lvj = RooRealVar('rrv_mass_lvj','rrv_mass_lvj',1000,binlo,binhi)
	#w		= fileInWs.Get('workspace4limit_')
	#rrv_mass_lvj	= w.var('rrv_mass_lvj')
	rrv_mass_lvj.SetTitle('M_{WV}')
	rrv_mass_lvj.setRange(binlo,binhi)

	SMPdf		= RooExponential('SMPdf_%s_%s'%(cat,ch),'SMPdf_%s_%s'%(cat,ch),rrv_mass_lvj,a1)
	SMdatahist	= RooDataHist('SMdatahist','SMdatahist',RooArgList(rrv_mass_lvj),SMhist)
	##actual fit to determine SM shape parameter
	fitresSM	= SMPdf.fitTo(SMdatahist, RooFit.SumW2Error(kTRUE))
	a1.setConstant(kTRUE)
	N_SM		= RooRealVar('N_SM_%s_%s'%(cat,ch),'N_SM_%s_%s'%(cat,ch),SMdatahist.sumEntries())
	N_SM.setConstant(kTRUE)

	getattr(wtmp,'import')(SMdatahist)
	getattr(wtmp,'import')(N_SM)


	#make and fill ATGC histograms, fit quadratic scales
	##do this for all 3 parameters
	for para in POI:
		hist4fit = TH1F('hist4fit_%s'%para,'hist4fit_%s'%para,3,-1.5*par_max[para],1.5*par_max[para])
		hist4fit.SetDirectory(0)
		pos_hist	= pos_hists[para]
		neg_hist	= neg_hists[para]
		pos_datahist	= RooDataHist('pos_datahist_%s'%para,'pos_datahist_%s'%para,RooArgList(rrv_mass_lvj),pos_hist)
		neg_datahist	= RooDataHist('neg_datahist_%s'%para,'neg_datahist_%s'%para,RooArgList(rrv_mass_lvj),neg_hist)
#
		hist4fit.SetBinContent(1,neg_datahist.sumEntries()/N_SM.getVal())
		hist4fit.SetBinContent(2,1)
		hist4fit.SetBinContent(3,pos_datahist.sumEntries()/N_SM.getVal())
		#fit parabel
		hist4fit.Fit('pol2')
		fitfunc		= hist4fit.GetFunction('pol2')
		par0		= RooRealVar('par0_%s_%s_%s'%(para,cat,ch),'par0_%s_%s_%s'%(para,cat,ch),fitfunc.GetParameter(0)); 		par0.setConstant(kTRUE);
		par1		= RooRealVar('par1_%s_%s_%s'%(para,cat,ch),'par1_%s_%s_%s'%(para,cat,ch),fitfunc.GetParameter(1)); 		par1.setConstant(kTRUE);
		par2		= RooRealVar('par2_%s_%s_%s'%(para,cat,ch),'par2_%s_%s_%s'%(para,cat,ch),fitfunc.GetParameter(2)); 		par2.setConstant(kTRUE);
#
		N_quad		= RooRealVar('N_quad_%s_%s_%s'%(para,cat,ch),'N_quad_%s_%s_%s'%(para,cat,ch), ((pos_datahist.sumEntries()+neg_datahist.sumEntries())/2)-N_SM.getVal() )
		#scaleshape is the relative change to SM		
		scaleshape	= RooFormulaVar('scaleshape_%s_%s_%s'%(para,cat,ch),'scaleshape_%s_%s_%s'%(para,cat,ch),\
						'(@0+@1*@3+@2*@3**2)-1',\
						RooArgList(par0,par1,par2,wtmp.var(para)))			

		a2		= RooRealVar('a_quad_%s_%s_%s'%(para,cat,ch),'a_quad_%s_%s_%s'%(para,cat,ch),-0.001,-1,0.1)
		a2.setConstant(kTRUE)
		cPdf_quad	= RooExponential('Pdf_quad_%s_%s_%s'%(para,cat,ch),'Pdf_quad_%s_%s_%s'%(para,cat,ch),rrv_mass_lvj,a2)

		getattr(wtmp,'import')(cPdf_quad)
		getattr(wtmp,'import')(pos_datahist)
		getattr(wtmp,'import')(neg_datahist)
		getattr(wtmp,'import')(N_quad)
		getattr(wtmp,'import')(scaleshape)
		wtmp.Print()


	#make model
	paralist	= RooArgList(N_SM)

	paralist.add(RooArgList(wtmp.function('N_quad_%s_%s_%s'%(POI[0],cat,ch)),wtmp.var('cwww'),\
				wtmp.function('N_quad_%s_%s_%s'%(POI[1],cat,ch)),wtmp.var('ccw'),\
				wtmp.function('N_quad_%s_%s_%s'%(POI[2],cat,ch)),wtmp.var('cb')))
	N1		= RooFormulaVar( 'N1_%s_%s'%(cat,ch),'N1_%s_%s'%(cat,ch),'@0/(@0+@1*(@2/12)**2+@3*(@4/20)**2+@5*(@6/60)**2)',paralist)
	N2		= RooFormulaVar( 'N2_%s_%s'%(cat,ch),'N2_%s_%s'%(cat,ch),'@1*(@2/12)**2/(@0+@1*(@2/12)**2+@3*(@4/20)**2+@5*(@6/60)**2)',paralist)
	N3		= RooFormulaVar( 'N3_%s_%s'%(cat,ch),'N3_%s_%s'%(cat,ch),'@3*(@4/20)**2/(@0+@1*(@2/12)**2+@3*(@4/20)**2+@5*(@6/60)**2)',paralist)
	N4		= RooFormulaVar( 'N4_%s_%s'%(cat,ch),'N4_%s_%s'%(cat,ch),'@5*(@6/60)**2/(@0+@1*(@2/12)**2+@3*(@4/20)**2+@5*(@6/60)**2)',paralist)
	N_list		= RooArgList(N1,N2,N3,N4)
	Pdf_list	= RooArgList(SMPdf,wtmp.pdf('Pdf_quad_%s_%s_%s'%(POI[0],cat,ch)),wtmp.pdf('Pdf_quad_%s_%s_%s'%(POI[1],cat,ch)),wtmp.pdf('Pdf_quad_%s_%s_%s'%(POI[2],cat,ch)))

	model		= RooAddPdf('aTGC_model','aTGC_model', Pdf_list, N_list)
	#model.Print()

	scale_list	= RooArgList(wtmp.function('scaleshape_%s_%s_%s'%(POI[0],cat,ch)),\
					wtmp.function('scaleshape_%s_%s_%s'%(POI[1],cat,ch)),\
					wtmp.function('scaleshape_%s_%s_%s'%(POI[2],cat,ch)))
	normfactor_3d	= RooFormulaVar('normfactor_3d_%s_%s'%(cat,ch),'normfactor_3d_%s_%s'%(cat,ch),'1+@0+@1+@2',scale_list)

	getattr(WS,'import')(normfactor_3d)	
	getattr(wtmp,'import')(normfactor_3d)	
	#wtmp.Print()
	
	#fit 3 pdfs
	fitresults	= []
	VocabularyWithResults = {}
	for i in range(3):
		wtmp.var(POI[0]).setVal(0); wtmp.var(POI[1]).setVal(0); wtmp.var(POI[2]).setVal(0);
		wtmp.var(POI[i]).setVal(-par_max[POI[i]])
		wtmp.var('a_quad_%s_%s_%s'%(POI[i],cat,ch)).setConstant(kFALSE)
		fitres		= model.fitTo(wtmp.data('neg_datahist_%s'%POI[i]),RooFit.Save(kTRUE), RooFit.SumW2Error(kTRUE))
		fitresults.append(fitres)
		wtmp.var('a_quad_%s_%s_%s'%(POI[i],cat,ch)).setConstant(kTRUE)

	for i in range(3):
		print "============================", fitresults[i].floatParsFinal().find('a_quad_%s_%s_%s'%(POI[i],cat,ch)).getVal()
		VocabularyWithResults['a_quad_%s_%s_%s'%(POI[i],cat,ch)] = fitresults[i].floatParsFinal().find('a_quad_%s_%s_%s'%(POI[i],cat,ch)).getVal()
		#fitresults[i].Print()
	
	return VocabularyWithResults


def main(options):
	if not options.input.endswith("/"):
		options.input += "/"
	fileWithHists = TFile(options.input + 'hists_signal_%s_%s.root'%(options.cat, options.ch))
	fileWithHists.cd()

	keyList = [key.GetName() for key in gDirectory.GetListOfKeys()]
	ListOfSystematics = []
	for k in keyList:
		if "SMhist" in k  and "Up" in k:
			ListOfSystematics.append(k.replace("SMhist_","").replace("Up",""))

	VocabularyForSystematicsUp = {}
	VocabularyForSystematicsDown = {}

	UncertaintiesUp = {}
	UncertaintiesDown = {}

	for iSyst in ListOfSystematics:
		VocabularyForSystematicsUp[iSyst] = {}
		VocabularyForSystematicsDown[iSyst] = {}

	UncertaintiesUp["a_quad_cb_" + options.cat + "_" + options.ch] = {}
	UncertaintiesUp["a_quad_ccw_" + options.cat + "_"+ options.ch] = {}
	UncertaintiesUp["a_quad_cwww_" + options.cat + "_"+ options.ch] = {}
	
	UncertaintiesDown["a_quad_cb_"+ options.cat + "_"+ options.ch] = {}
	UncertaintiesDown["a_quad_ccw_"+ options.cat + "_"+ options.ch] = {}
	UncertaintiesDown["a_quad_cwww_"+ options.cat +"_"+ options.ch] = {}

	UncertaintiesSquaredUp = {}
	UncertaintiesSquaredUp["a_quad_cb_"+ options.cat +"_"+ options.ch] = 0.
	UncertaintiesSquaredUp["a_quad_ccw_"+ options.cat +"_"+ options.ch] = 0.
	UncertaintiesSquaredUp["a_quad_cwww_"+ options.cat +"_"+ options.ch] = 0.

	UncertaintiesSquaredDown = {}
	UncertaintiesSquaredDown["a_quad_cb_"+ options.cat +"_"+ options.ch] = 0.
	UncertaintiesSquaredDown["a_quad_ccw_"+ options.cat +"_"+ options.ch] = 0.
	UncertaintiesSquaredDown["a_quad_cwww_"+ options.cat +"_"+ options.ch] = 0.

	Uncertainties = {}

	SMhist = fileWithHists.Get("SMhist")
	pos_hists = {}
	neg_hists = {}
	for para in POI:
		pos_hists[para] = fileWithHists.Get('signalPositive_%s'%para)
		neg_hists[para] = fileWithHists.Get('signalNegative_%s'%para)
		
	#get nominal values
	NominalValues = getSignalParmeters(options.cat, SMhist, pos_hists, neg_hists, options.ch)
	#get values for systematics
	for iSyst in ListOfSystematics:
		#start with Up variation
		SMhist = fileWithHists.Get("SMhist_"+iSyst + "Up")
		for para in POI:
			pos_hists[para] = fileWithHists.Get('signalPositive_%s'%para+"_" + iSyst + "Up")
			neg_hists[para] = fileWithHists.Get('signalNegative_%s'%para+"_" + iSyst + "Up")

		VocabularyForSystematicsUp[iSyst] = getSignalParmeters(options.cat, SMhist, pos_hists, neg_hists, options.ch)
		print VocabularyForSystematicsUp

		SMhist = fileWithHists.Get("SMhist_"+iSyst + "Up")
		for para in POI:
			pos_hists[para] = fileWithHists.Get('signalPositive_%s'%para+"_" + iSyst + "Down")
			neg_hists[para] = fileWithHists.Get('signalNegative_%s'%para+"_" + iSyst + "Down")
		VocabularyForSystematicsDown[iSyst] = getSignalParmeters(options.cat, SMhist, pos_hists, neg_hists, options.ch)	
		#print VocabularyForSystematicsDown	

	for iSyst in VocabularyForSystematicsUp:
		for iATGC in VocabularyForSystematicsUp[iSyst]:
			UncertaintiesSquaredUp[iATGC] += pow(abs(VocabularyForSystematicsUp[iSyst][iATGC] - NominalValues[iATGC]),2)
			UncertaintiesSquaredDown[iATGC] += pow(abs(VocabularyForSystematicsDown[iSyst][iATGC] - NominalValues[iATGC]),2)

	for iATGC in UncertaintiesSquaredUp:
		UncertaintiesUp[iATGC] = sqrt(UncertaintiesSquaredUp[iATGC])
		UncertaintiesDown[iATGC] = sqrt(UncertaintiesSquaredDown[iATGC])
		Uncertainties[iATGC] = format(abs(100*(max(UncertaintiesUp[iATGC], UncertaintiesDown[iATGC]))/NominalValues[iATGC]),".2f")

	print Uncertainties
	tdrstyle.setTDRStyle()
	canvas = TCanvas("canvas","canvas",1200,800)
	canvas.SetLogy()

	low = 900.
	high = 3500.
	step = (high - low)/1000
	for iATGC in POI:
		legend = TLegend(0.7,0.7,0.9,0.8)
		legend.SetFillColor(kWhite)
		if options.ch == "ele":
			legend.SetHeader(options.cat + " , electron channel")
		elif options.ch == "mu" :
			legend.SetHeader(options.cat + " , muon channel")
		else :
			raise RuntimeError('channel not supported!')
		integral = (exp(high*NominalValues["a_quad_" + iATGC + "_" + options.cat + "_" + options.ch ]) - exp(low*NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch ]))/NominalValues["a_quad_" + iATGC + "_" + options.cat + "_" + options.ch ]

		ValueUp = NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch] + UncertaintiesUp["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch]
		ValueDown = NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch] - UncertaintiesDown["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch]

		integralUp = (exp(high*ValueUp) - exp(low*ValueUp))/ValueUp
		integralDown = (exp(high*ValueDown) - exp(low*ValueDown))/ValueDown

		mass = low
		iMass = 0
		graph = TGraphAsymmErrors()
		sum_ = 0.
		while mass <= high:
			graph.SetPoint(iMass, mass, pow(integral,-1)*exp(NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch]*mass))
			graph.SetPointEYlow(iMass,  abs(pow(integral,-1)*exp(NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch]*mass)  - pow(integralDown,-1)*exp((NominalValues["a_quad_"+ iATGC + "_"+ options.cat +"_"+ options.ch] - UncertaintiesDown["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch])*mass )))
			graph.SetPointEYhigh(iMass,  abs(pow(integral,-1)*exp(NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch]*mass)  - pow(integralUp,-1)*exp((NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch] + UncertaintiesUp["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch])*mass )))
			mass += step
			sum_ += pow(integral,-1)*exp(NominalValues["a_quad_"+ iATGC +"_"+ options.cat +"_"+ options.ch]*mass)*step
			iMass += 1
		graph.SetFillStyle(3010)
		graph.GetXaxis().SetTitle("m_{WV}")
		graph.GetYaxis().SetTitle("arb. units")
		graph.SetLineWidth(4)
		graph.SetLineColor(kRed)
		legend.AddEntry(graph, iATGC,"l")
		graph.Draw("AL3")
		hist_ = fileWithHists.Get('signalNegative_%s'%iATGC) 
		hist_.SetLineWidth(4)
		hist_.SetLineColor(kBlue)
		SetSystematicsFromFile(fileWithHists,hist_, ListOfSystematics)
		hist_.Add(SMhist, -1.)
		hist_.Scale(1/hist_.Integral("width"))
		graphMC = TGraph()
		for iBin in range(1, hist_.GetNbinsX()+1):
			graphMC.SetPoint(iBin, hist_.GetBinCenter(iBin),hist_.GetBinContent(iBin) )
		graphMC.SetMarkerColor(kBlue)
		legend.AddEntry(graphMC, "MC", "p")		
		graphMC.Draw("PSAME")
		legend.Draw("SAME")
		

		CMS_lumi.CMS_lumi(canvas, 4, 33)
		canvas.SaveAs("graph_"+ iATGC + "_" + options.cat + "_" + options.ch +".png")
		canvas.Clear()
		


if __name__ == "__main__":
	parser	= OptionParser()	
	parser.add_option('--cat', dest='cat', default='WW', help='category, WW or WZ, defines signal region')
	parser.add_option('--ch', dest='ch', default='mu', help='channel, electron or muon')
	parser.add_option('--input', dest='input', default='', help='input gDirectory')

	(options,args) = parser.parse_args()
	main(options)




