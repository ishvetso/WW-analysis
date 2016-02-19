import os
import sys
import fileinput
import optparse
import sys

# Author: Ivan Shvetsov
# Numquam ponenda est pluralitas sine necessitate

def createFileForJob(processName, channel, sampleName, feature, configDIR, outDir, YourJSONFile, RunRange, isMC, isSignal, wantToSubmit=False, fileName='template.txt'):
	if not os.path.exists(outDir):
    		os.makedirs(outDir)

	patternFeature = "$FEATURE$"
	patternProcessName = "$PROCESS$"
	patternChannel = "$CHANNEL$"
	patternSample = "$SAMPLE$"
	patternMCvsData= "$MCvsDATA$"
	configDIRToSearch = "$configDIR$"
	patternJobSplitting = "$JOBSplitting$"
	patternForDataOnly = "$IsData$"
	patterUnitPerJob = "$UNITSPERJOB$"


	fileTemplate = 'templates/' + fileName
	tempFile = open( outDir + "/" + processName + "-" + channel + ".py", 'w+' )
	infile = open(fileTemplate)
	for line in infile:
		if patternFeature in line and patternChannel in line and patternProcessName in line:
			tempFile.write( line.replace( patternFeature, feature).replace(patternProcessName, processName).replace(patternChannel, channel))
		elif configDIRToSearch in line and patternChannel in line and patternMCvsData:
			if isMC and not isSignal :
				tempFile.write( line.replace( configDIRToSearch, configDIR).replace(patternChannel, channel).replace(patternMCvsData,"_MC"))
			elif isMC and isSignal:
				tempFile.write( line.replace( configDIRToSearch, configDIR).replace(patternChannel, channel).replace(patternMCvsData,"_signal"))
			elif not isMC :
				tempFile.write( line.replace( configDIRToSearch, configDIR).replace(patternChannel, channel).replace(patternMCvsData,""))
			else:
				raise ValueError('This should not happen!')
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
				tempFile.write( line.replace( patterUnitPerJob, "2"))
			else :
				tempFile.write( line.replace( patterUnitPerJob, "20"))
		else:
			tempFile.write( line)
	tempFile.close()
	if wantToSubmit :
		os.system("crab submit -c " + os.path.dirname(os.path.abspath(__file__)) + "/" + outDir + "/"  + processName + "-" + channel + ".py")
		print "\033[0;40;32mtask:", processName + "-" + channel, "was submitted!\033[0m"
	return;


parser =  optparse.OptionParser()
parser.add_option('-p', '--Feature', dest="Feature", default='my_feature')
parser.add_option('-d', '--dir', dest="directory", default='/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_6_3/src/aTGCsAnalysis/Common/')
(options, arg) = parser.parse_args()


def submitJobs(MCBackgroundsSampleDictionary, SignalMCSampleDictionary, DataDictionaryElectronChannel, DataDictionaryMuonChannel, JSONFile, YourRunRange,wantToSubmit=False):
	#for key in MCBackgroundsSampleDictionary:
		createFileForJob(key, "mu", MCBackgroundsSampleDictionary[key], options.Feature, options.directory,  "crabConfigs",  JSONFile, YourRunRange, True, False, wantToSubmit)
		createFileForJob(key, "ele", MCBackgroundsSampleDictionary[key], options.Feature, options.directory,  "crabConfigs",  JSONFile, YourRunRange, True, False, wantToSubmit)
	#for key in SignalMCSampleDictionary:
		createFileForJob(key, "mu", SignalMCSampleDictionary[key], options.Feature, options.directory,  "crabConfigs",  JSONFile, YourRunRange, True, True, wantToSubmit)
		createFileForJob(key, "ele", SignalMCSampleDictionary[key], options.Feature, options.directory,  "crabConfigs",  JSONFile, YourRunRange, True, True, wantToSubmit)
	for key in DataDictionaryElectronChannel:
		createFileForJob(key, "ele", DataDictionaryElectronChannel[key], options.Feature, options.directory,  "crabConfigs",  JSONFile, YourRunRange, False, True, wantToSubmit)
	for key in DataDictionaryMuonChannel:
		createFileForJob(key, "mu", DataDictionaryMuonChannel[key], options.Feature, options.directory,  "crabConfigs",  JSONFile, YourRunRange, False, True, wantToSubmit)


MCBackgroundsSampleDictionary =    {'ttbar':'/TTJets_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM',
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

DataDictionaryMuonChannel = {'data-RunC':'/SingleMuon/Run2015C_25ns-16Dec2015-v1/MINIAOD',
							 'data-RunD':'/SingleMuon/Run2015D-16Dec2015-v1/MINIAOD'}

DataDictionaryElectronChannel = {'data-RunC':'/SingleElectron/Run2015C_25ns-16Dec2015-v1/MINIAOD',
								 'data-RunD':'/SingleElectron/Run2015D-16Dec2015-v1/MINIAOD'}

MyJSON = "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions15/13TeV/Reprocessing/Cert_13TeV_16Dec2015ReReco_Collisions15_25ns_JSON.txt"
	

submitJobs(MCBackgroundsSampleDictionary, SignalMCSampleDictionary, DataDictionaryElectronChannel, DataDictionaryMuonChannel, MyJSON,"254000-261000", False)
