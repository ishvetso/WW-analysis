import os
import sys
import fileinput
import optparse
import sys
import re

#This is a script to make a life of analyzer easier. 
# 1) check the status of the task
# 2) if the task is 100% complete retrieve it
# 3) merge files and clean the directory
# 4) move the file the configurable directory

# Author: Ivan Shvetsov, 5 of November 2015
# Numquam ponenda est pluralitas sine necessitate

def checkTaskStatus(taskName):
	tmp = os.popen("crab status crab_projects/crab_" + taskName +  " | grep 'finished'").read()
	print "\033[0;40;32m checking the status of the task : " + taskName +  "\033[0m"
	print "\033[0;40;32m " + tmp + " \033[0m"
	return re.findall("\d+", tmp)[0]


def RetrieveTask(taskName, outputFileName, outDir):
	if checkTaskStatus(taskName) == '100':
 		tmpOut = os.popen("crab getoutput crab_projects/crab_" + taskName ).read()
 		if tmpOut.find("All files successfully retrieved") != -1 :
 			print "\033[0;40;32mAll files successfully retrieved : " + taskName +  "\033[0m"
 		else :
 			print "\033[0;40;31mNot all of files were retrieved, let's try once more! \033[0m"
 			tmpOut = os.popen("crab getoutput crab_projects/crab_" + taskName ).read()
 			if tmpOut.find("All files successfully retrieved") != -1 :
 				print "\033[0;40;32mAll files successfully retrieved : " + taskName +  "\033[0m"
 			else :
 				print "\033[0;40;31mAfter 2 attempts not all of files were retrieved! \033[0m"
 				sys.exit(0)
 		os.chdir("crab_projects/crab_" + taskName + "/results/")
 		os.system("hadd " + outputFileName + ".root" + "  tree_*.root")
 		os.system("rm tree_*.root")
 		os.system("mv " + outputFileName + ".root  " +  outDir )
 		os.chdir("../../../")
 		print "\033[0;40;32m task : "  +  taskName +  " retrieved successfully. \033[0m"
 	else :
 		os.system("crab resubmit -d crab_projects/crab_" + taskName)	
 		print "\033[0;40;31m task is not retrieved as it's not 100% finished : " + taskName +  "\033[0m"



TaskDictionaryName = {
	"SingleTop-s-channel":"s-ch",
	"SingleTop-t-channel":"t-ch",
	"SingleTop-tW-channel-top": "tW-ch-top",
	"SingleTop-tW-channel-antitop": "tW-ch-antitop",
	"WW":"WW",
	"WZ":"WZ",
	"ttbar": "ttbar",
	"WJets_HT-100To200":"WJets_Ht100To200",
	"WJets_HT-200To400":"WJets_Ht200To400",
	"WJets_HT-400To600":"WJets_Ht400To600",
	"WJets_HT-600To800":"WJets_Ht600To800",
	"WJets_HT-800To1200":"WJets_Ht800To1200",
	"WJets_HT1200To2500":"WJets_Ht1200To2500",
	"WJets_HT2500ToInf":"WJets_Ht2500ToInf",
	"WW-signal":"WW-aTGC",	
	"WZ-signal":"WZ-aTGC",	
	"data-RunD":"data-RunD",
}


def Retrieval(feature, outDir):
	if not os.path.exists(outDir):
		os.makedirs(outDir)

	for TaskName, OutName in TaskDictionaryName.items():
		RetrieveTask(TaskName + "_mu_" + feature, OutName + "_mu", outDir)
		RetrieveTask(TaskName + "_ele_" + feature, OutName + "_ele", outDir )


Retrieval("76X_reprocessing", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_31March2016/" )
