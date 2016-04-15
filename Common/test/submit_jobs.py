import os
import sys
import fileinput
import optparse
import sys
import shutil
import subprocess

# Author: Ivan Shvetsov
# Numquam ponenda est pluralitas sine necessitate

def DefineNJobs(sample): 
	ps = subprocess.Popen(['./das_client.py', '--query', "file dataset=" + sample + " | count(file.name)"], stdout=subprocess.PIPE)
	output = ps.communicate()[0]
	for line in output.splitlines():
		if "count(file.name)=" in line :
			replacedline = line.replace("count(file.name)=","")
	N = int(replacedline)
	
	if N < 100 :
		NFilesPerJob = 1
	else :
		# let's roughly assume that number of files is ~1000 in the worst case
		NFilesPerJob = int(N/100)
	print "N of files per job : " , sample , " " , NFilesPerJob	
	return NFilesPerJob


def createConfigFile(processName, channel, isMC, isSignal):
	if not os.path.exists("analysisConfigs"):
		os.makedirs("analysisConfigs")
	BTagEfficiencyPattern = "BtagEffFile = cms.string(\"\"),\n"
	VTagSFPattern = "VTagSF = cms.double(0.915)"
	ConfigFileName = ""
   	if isSignal and isMC :
   		shutil.copy("../analysis_" + channel + "_signal.py", "analysisConfigs")
   		ConfigFileName = "analysisConfigs/analysis_" + channel + "_signal.py"
   	elif processName == "ttbar-amATNLO" or processName == "ttbar-powheg" or processName == "WZ":
   		shutil.copy("../analysis_" + channel + "_MC.py", "analysisConfigs/analysis_" + channel + "_" + processName + ".py")
   		ConfigFileName = "analysisConfigs/analysis_" + channel + "_" + processName + ".py"
   		configFile = open("../analysis_" + channel + "_MC.py")
   		outFile = open("analysisConfigs/analysis_" + channel + "_" + processName + ".py", "w+")
   		for line in configFile:
   			if BTagEfficiencyPattern in line :
   				replaceWith = "BtagEffFile = cms.string(\"aTGCsAnalysis/TreeMaker/data/eff_" + processName + "_" + channel + ".root\"),\n"
   				outFile.write(line.replace(BTagEfficiencyPattern, replaceWith ))
   			else :
   				outFile.write(line)
   	# don't apply V-tagging scale factor for W+jets or single top t-channel and s-channel
   	elif processName == "WJets_HT-100To200" or processName == "WJets_HT-200To400" or processName == "WJets_HT-400To600" or processName == "WJets_HT-600To800" or processName == "WJets_HT-800To1200" or processName == "WJets_HT-1200To2500" or processName == "WJets_HT-2500ToInf" or processName == "SingleTop-s-channel" or processName == "SingleTop-t-channel":
   		shutil.copy("../analysis_" + channel + "_MC.py", "analysisConfigs/analysis_" + channel + "_" + processName + ".py")
   		ConfigFileName = "analysisConfigs/analysis_" + channel + "_" + processName + ".py"
   		configFile = open("../analysis_" + channel + "_MC.py")
   		outFile = open("analysisConfigs/analysis_" + channel + "_" + processName + ".py", "w+")
   		for line in configFile:
   			if VTagSFPattern in line :
   				replaceWith = "VTagSF = cms.double(1.0)"
   				outFile.write(line.replace(VTagSFPattern, replaceWith ))
   			else :
   				outFile.write(line)

   	elif isMC :
   		shutil.copy("../analysis_" + channel + "_MC.py", "analysisConfigs")
   		ConfigFileName = "analysisConfigs/analysis_" + channel + "_MC.py"
   	elif not isMC :
   		shutil.copy("../analysis_" + channel + ".py", "analysisConfigs")
   		ConfigFileName = "analysisConfigs/analysis_" + channel + ".py"
   	else :
   		raise ValueError('This should not happen!')
   	return os.path.abspath(ConfigFileName)


def createFileForJob(processName, channel, sampleName, feature, configFileName, outDir, YourJSONFile, RunRange, isMC, isSignal, wantToSubmit=False, fileName='template.txt'):
	if not os.path.exists(outDir):
    		os.makedirs(outDir)

	patternFeature = "$FEATURE$"
	patternProcessName = "$PROCESS$"
	patternChannel = "$CHANNEL$"
	patternSample = "$SAMPLE$"
	patternConfigFile= "$CONFIGFILENAME$"
	patternJobSplitting = "$JOBSplitting$"
	patternForDataOnly = "$IsData$"
	patterUnitPerJob = "$UNITSPERJOB$"


	fileTemplate = 'templates/' + fileName
	tempFile = open( outDir + "/" + processName + "-" + channel + ".py", 'w+' )
	infile = open(fileTemplate)
	for line in infile:
		if patternFeature in line and patternChannel in line and patternProcessName in line:
			tempFile.write( line.replace( patternFeature, feature).replace(patternProcessName, processName).replace(patternChannel, channel))
		elif patternConfigFile in line :
			tempFile.write( line.replace( patternConfigFile, configFileName))		
		elif patternSample in line:
			tempFile.write( line.replace( patternSample, sampleName))
		elif patternJobSplitting in line:
			if isMC :
				tempFile.write( line.replace( patternJobSplitting, "FileBased"))
			else :
				tempFile.write( line.replace( patternJobSplitting, "LumiBased"))
		elif patternForDataOnly in line:
			if not isMC:
				tempFile.write( "config.Data.lumiMask = '" + YourJSONFile + "'\n")
				tempFile.write( "config.Data.runRange = '" + RunRange + "'\n")
			else :
				continue
		elif patterUnitPerJob in line :
			if isMC :
				NUnitPerJobWise = DefineNJobs(sampleName)
				tempFile.write( line.replace( patterUnitPerJob, str(NUnitPerJobWise)))
			else :
				tempFile.write( line.replace( patterUnitPerJob, "50"))
		else:
			tempFile.write( line)
	tempFile.close()
	if wantToSubmit :
		os.system("crab submit -c " + os.path.dirname(os.path.abspath(__file__)) + "/" + outDir + "/"  + processName + "-" + channel + ".py")
		print "\033[0;40;32mtask:", processName + "-" + channel, "was submitted!\033[0m"
	return;


parser =  optparse.OptionParser()
parser.add_option('-p', '--Feature', dest="Feature", default='my_feature')
(options, arg) = parser.parse_args()


def submitJobs(MCBackgroundsSampleDictionary, SignalMCSampleDictionary, DataDictionaryElectronChannel, DataDictionaryMuonChannel, JSONFile, YourRunRange,wantToSubmit=False):
	for key in MCBackgroundsSampleDictionary:
		ConfigFileName = createConfigFile(key, "mu", True, False)
		print key, " ", ConfigFileName
		createFileForJob(key, "mu", MCBackgroundsSampleDictionary[key], options.Feature, ConfigFileName,  "crabConfigs",  JSONFile, YourRunRange, True, False, wantToSubmit)
		ConfigFileName = createConfigFile(key, "ele", True, False)
		createFileForJob(key, "ele", MCBackgroundsSampleDictionary[key], options.Feature, ConfigFileName,  "crabConfigs",  JSONFile, YourRunRange, True, False, wantToSubmit)
	for key in SignalMCSampleDictionary:
		ConfigFileName = createConfigFile(key, "mu", True, True)
		createFileForJob(key, "mu", SignalMCSampleDictionary[key], options.Feature, ConfigFileName,  "crabConfigs",  JSONFile, YourRunRange, True, True, wantToSubmit)
		ConfigFileName = createConfigFile(key, "ele", True, True)
		createFileForJob(key, "ele", SignalMCSampleDictionary[key], options.Feature, ConfigFileName,  "crabConfigs",  JSONFile, YourRunRange, True, True, wantToSubmit)
	for key in DataDictionaryElectronChannel:
		ConfigFileName = createConfigFile(key, "ele", False, False)
		createFileForJob(key, "ele", DataDictionaryElectronChannel[key], options.Feature, ConfigFileName,  "crabConfigs",  JSONFile, YourRunRange, False, True, wantToSubmit)
	for key in DataDictionaryMuonChannel:
		ConfigFileName = createConfigFile(key, "mu", False, False)
		createFileForJob(key, "mu", DataDictionaryMuonChannel[key], options.Feature, ConfigFileName,  "crabConfigs",  JSONFile, YourRunRange, False, True, wantToSubmit)


MCBackgroundsSampleDictionary =    {'ttbar-amATNLO':'/TTJets_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM',
									'ttbar-powheg':'/TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext4-v1/MINIAODSIM',
									'WJets_HT-100To200':'/WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WJets_HT-200To400':'/WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WJets_HT-400To600':'/WJetsToLNu_HT-400To600_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WJets_HT-600To800':'/WJetsToLNu_HT-600To800_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WJets_HT-800To1200':'/WJetsToLNu_HT-800To1200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WJets_HT-1200To2500':'/WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WJets_HT-2500ToInf':'/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WW':'/WWToLNuQQ_13TeV-powheg/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'WZ':'/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'SingleTop-s-channel':'/ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v2/MINIAODSIM',
									'SingleTop-t-channel':'/ST_t-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'SingleTop-tW-channel-antitop':'/ST_tW_antitop_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
									'SingleTop-tW-channel-top':'/ST_tW_top_5f_inclusiveDecays_13TeV-powheg-pythia8_TuneCUETP8M1/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'}

SignalMCSampleDictionary = {'WW-signal':'/WWToLNuQQ_aTGC_13TeV-madgraph-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM',
							'WZ-signal': '/WZToLNu2QorQQ2L_aTGC_13TeV-madgraph-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM'}

DataDictionaryMuonChannel = {'data-RunD':'/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD'}

DataDictionaryElectronChannel = {'data-RunD':'/SingleElectron/Run2015D-16Dec2015-v1/MINIAOD'}

MyJSON = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt"
	

submitJobs(MCBackgroundsSampleDictionary, SignalMCSampleDictionary, DataDictionaryElectronChannel, DataDictionaryMuonChannel, MyJSON,"254000-261000", True)
