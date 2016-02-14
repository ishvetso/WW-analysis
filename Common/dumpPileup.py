import ROOT
import os
from SimGeneral.MixingModule.mix_2015_25ns_FallMC_matchData_PoissonOOTPU_cfi import mix as mix_2015_25ns_realistScenario


filePileupData = ROOT.TFile("MyDataPileupHistogram.root")
histPUData = filePileupData.Get("pileup")
histPUData.Scale(1./histPUData.Integral())
for iBin in range(1, histPUData.GetNbinsX()):
	print histPUData.GetBinContent(iBin), ","

#print sum(mix_2015_25ns_realistScenario.input.nbPileupEvents.probValue)
