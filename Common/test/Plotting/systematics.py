import ROOT
import os
from optparse import OptionParser
import sys

def main():
	f = ROOT.TFile("/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_6_4/src/aTGCsAnalysis/Common/test/Plotting/hists.root")
	f.cd()
	keyList = [key.GetName() for key in ROOT.gDirectory.GetListOfKeys()]
	nominalIntegrals ={}
	processName = ""
	#first fill nominal integrals
	for k in keyList:
		obj = ROOT.gDirectory.GetKey(k).ReadObj()
		if "Up" not in k and "Down" not in k:
			nominalIntegrals[k.replace("_MWW","")]=obj.Integral()
	print nominalIntegrals
	#make a loop over the process and calculate uncertainties
	for iProcess in nominalIntegrals:
		UpUncertainty = 0.0;
		DownUncertainty = 0.0;
		for k in keyList:
			if iProcess in k and "Up" in k :
				obj = ROOT.gDirectory.GetKey(k).ReadObj()
				upIntegral = obj.Integral()
				UpUncertainty += upIntegral - nominalIntegrals[iProcess]
			elif iProcess in k and "Down" in k :
				obj = ROOT.gDirectory.GetKey(k).ReadObj()
				downIntegral = obj.Integral()
				DownUncertainty += downIntegral - nominalIntegrals[iProcess]
			else :
				continue
		print iProcess, " nominal: ", nominalIntegrals[iProcess], " up : ", UpUncertainty, " down : ", DownUncertainty


if __name__ == "__main__": main()	