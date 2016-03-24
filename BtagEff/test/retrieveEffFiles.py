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
	tmp = os.popen("crab status crabConfigs/crab_projects/crab_" + taskName +  " | grep 'finished'").read()
	print "\033[0;40;32m checking the status of the task : " + taskName +  "\033[0m"
	print "\033[0;40;32m " + tmp + " \033[0m"
	print re.findall("\d+", tmp) 
	return re.findall("\d+", tmp)[0]


def RetrieveTask(taskName, outputFileName, outDir):
	if checkTaskStatus(taskName) == '100':
 		#os.system("crab getoutput crabConfigs/crab_projects/crab_" + taskName)
 		tmpOut = os.popen("crab getoutput crabConfigs/crab_projects/crab_" + taskName ).read()
 		if tmpOut.find("All files successfully retrieved") != -1 :
 			print "\033[0;40;32mAll files successfully retrieved : " + taskName +  "\033[0m"
 		else :
 			print "\033[0;40;31mNot all of files were retrieved, let's try once more! \033[0m"
 			tmpOut = os.popen("crab getoutput crabConfigs/crab_projects/crab_" + taskName ).read()
 			if tmpOut.find("All files successfully retrieved") != -1 :
 				print "\033[0;40;32mAll files successfully retrieved : " + taskName +  "\033[0m"
 			else :
 				print "\033[0;40;31mAfter 2 attempts not all of files were retrieved! \033[0m"
 				sys.exit(0)
 		os.chdir("crabConfigs/crab_projects/crab_" + taskName + "/results/")
 		os.system("hadd " + outputFileName + ".root" + "  tree_*.root")
 		os.system("rm tree_*.root")
 		os.system("mv " + outputFileName + ".root  " +  outDir )
 		os.chdir("../../../../")
 		print "\033[0;40;32m task : "  +  taskName +  " retrieved successfully. \033[0m"
 	else :
 		os.system("crab resubmit -d crabConfigs/crab_projects/crab_" + taskName)	
 		print "\033[0;40;31m task is not retrieved as it's not 100% finished : " + taskName +  "\033[0m"


TaskDictionaryName = {
	"WW":"WW",
	"WZ":"WZ",
	"WW-signal": "WW-aTGC",
	"WZ-signal": "WZ-aTGC",
	"ttbar": "ttbar",
}


def Retrieval(feature, outDir):
	if not os.path.exists(outDir):
		os.makedirs(outDir)

	for TaskName, OutName in TaskDictionaryName.items():
		RetrieveTask(TaskName  +  "_" + feature + "_mu", OutName + "_mu", outDir)
		RetrieveTask(TaskName  +  "_" + feature + "_ele", OutName + "_ele", outDir )


Retrieval("btag_eff", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/btagEff/" )
