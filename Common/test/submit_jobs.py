import os
import sys
import fileinput
import optparse
import sys

def createFileForJob(textToReplace, configDIR, fileName, outDir):
	if not os.path.exists(outDir):
    		os.makedirs(outDir)
	textToSearch = "$FEATURE$"
	configDIRToSearch = "$configDIR$" 
	fileTemplate = 'templates/' + fileName
	tempFile = open( outDir + "/" + fileName, 'w+' )
	infile = open(fileTemplate)
	for line in infile:
		if textToSearch in line:
			tempFile.write( line.replace( textToSearch, textToReplace))
		elif configDIRToSearch in line:
			tempFile.write( line.replace( configDIRToSearch, configDIR))
		else:
			tempFile.write( line)
	tempFile.close()
	return;


parser =  optparse.OptionParser()
parser.add_option('-p', '--pattern', dest="pattern", default='my_feature')
parser.add_option('-d', '--dir', dest="directory", default='/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_4_14/src/aTGCsAnalysis/Common/')
(options, arg) = parser.parse_args()

MyListOfJobs = ["crabConfig_STop_s-channel_mu.py", "crabConfig_STop_s-channel_ele.py", "crabConfig_STop_t-channel_antitop_ele.py", "crabConfig_STop_t-channel_antitop_mu.py", "crabConfig_STop_t-channel_top_ele.py",
 "crabConfig_STop_t-channel_top_mu.py", "crabConfig_STop_tW-channel_ele.py", "crabConfig_STop_tW-channel_mu.py", "crabConfig_WW_mu.py", "crabConfig_WW_ele.py", "crabConfig_WZ_mu.py",
  "crabConfig_WZ_ele.py", "crabConfig_TTBar_mu.py", "crabConfig_TTBar_ele.py","crabConfig_WJetsHt100To200_ele.py", "crabConfig_WJetsHt100To200_mu.py",
   "crabConfig_WJetsHt200To400_ele.py", "crabConfig_WJetsHt200To400_mu.py", "crabConfig_WJetsHt400To600_ele.py", "crabConfig_WJetsHt400To600_mu.py",
    "crabConfig_WJetsHt600ToInf_ele.py", "crabConfig_WJetsHt600ToInf_mu.py"]

def submitJobs(ListOfJobs):
	for iList in ListOfJobs:
		print iList
		createFileForJob(options.pattern, options.directory, iList, "crabConfigs")
		os.system("crab submit -c  " + "crabConfigs/" + iList)

submitJobs(MyListOfJobs)
