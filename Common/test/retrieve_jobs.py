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

def checkTaskStatus(taskName):
	tmp = os.popen("crab status crab_projects/crab_" + taskName +  " | grep 'finished'").read()
	print "\033[0;40;32m checking the status of the task : " + taskName +  "\033[0m"
	print "\033[0;40;32m " + tmp + " \033[0m"
	return re.findall("\d+", tmp)[0]


def RetrieveTask(taskName, outputFileName, outDir):
	if checkTaskStatus(taskName) == '100':
 		os.system("crab getoutput crab_projects/crab_" + taskName)
 		os.chdir("crab_projects/crab_" + taskName + "/results/")
 		os.system("hadd " + outputFileName + ".root" + "  tree_*.root")
 		os.system("rm tree_*.root")
 		os.system("mv " + outputFileName + ".root  " +  outDir )
 		os.chdir("../../..")
 		print "\033[0;40;32m task : "  +  taskName +  " retrieved successfully. \033[0m"
 	else :
 		print "\033[0;40;31m task is not retrieved as it's not 100% finished : " + taskName +  "\033[0m"


#RetrieveTask("WJets_Ht400To600_ele_4November2015", "WJets_Ht400To600_ele","/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_5November2015/" )

TaskDictionaryName = {
	"STop_s_channel_analysis":"s-ch",
	"STop_t_channel_antitop":"t-ch_top",
	"STop_t_channel_top": "t-ch_antitop",
	"STop_tW_channel": "tW-ch",
	"WW":"WW",
	"WZ":"WZ",
	"ttbar": "ttbar",
	"WJets_Ht100To200":"WJets_Ht100To200",
	"WJets_Ht200To400":"WJets_Ht200To400",
	"WJets_Ht400To600":"WJets_Ht400To600",
	"WJets_Ht600ToInf":"WJets_Ht600ToInf"	
}


def Retrieval(feature, outDir):
	for TaskName, OutName in TaskDictionaryName.items():
		RetrieveTask(TaskName + "_mu_" + feature, OutName + "_mu", outDir)
		RetrieveTask(TaskName + "_ele_" + feature, OutName + "_ele", outDir )


Retrieval("4November2015", "/afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_5November2015/" )
