import ROOT
import os
from optparse import OptionParser
import sys
import pandas as pd

def main(options):
	f = ROOT.TFile(options.input)
	f.cd()
	keyList = [key.GetName() for key in ROOT.gDirectory.GetListOfKeys()]
	nominalIntegrals ={}
	ListOfSystematics = []
	ProcessNames = []
	processName = ""
	#first fill nominal integrals
	for k in keyList:
		obj = ROOT.gDirectory.GetKey(k).ReadObj()
		if "Up" not in k and "Down" not in k:
			nominalIntegrals[k.replace("_MWW","")]=obj.Integral()
			processName = k.replace("_MWW","")
		if "Up" in k:
			ListOfSystematics.append(k.replace(processName+"_","").replace("Up",""))

	#erase duplicates
	ListOfSystematics = sorted(set(ListOfSystematics))
	VocabularyForSystematics = {}
	#create a list per syst
	for iSyst in ListOfSystematics:
		VocabularyForSystematics[iSyst] = []


	#make a loop over the process and calculate uncertainties
	for iProcess in nominalIntegrals:
		UpUncertainty = 0.0
		DownUncertainty = 0.0
		ProcessNames.append(iProcess)
		for iSyst in ListOfSystematics :
			for k in keyList:
				if iProcess in k and iSyst in k and "Up" in k  :
					obj = ROOT.gDirectory.GetKey(k).ReadObj()
					upIntegral = obj.Integral()
					UpUncertainty = upIntegral - nominalIntegrals[iProcess]
				elif iProcess in k  and iSyst in k and "Down" in k :
					obj = ROOT.gDirectory.GetKey(k).ReadObj()
					downIntegral = obj.Integral()
					DownUncertainty = downIntegral - nominalIntegrals[iProcess]
				else :
					continue
			absUnc = 100*max(UpUncertainty, DownUncertainty)/nominalIntegrals[iProcess]
			VocabularyForSystematics[iSyst].append(format(absUnc,'.2f'))

	#print VocabularyForSystematics
	table =  pd.DataFrame(VocabularyForSystematics,index=ProcessNames, columns=ListOfSystematics)
	if (not options.latex):
		print table
	else : 
		print table.to_latex()
if __name__ == "__main__":
	parser = OptionParser()
	parser.add_option("-i", "--input", default="efficiency-mc-GsfElectronToId.root", help="Input filename")
	parser.add_option("-l", action="store_true", dest="latex")
	(options, arg) = parser.parse_args()
	main(options)	