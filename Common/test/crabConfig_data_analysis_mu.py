from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = 'FirstDataSingleMu'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/analysis_mu.py'

config.Data.inputDataset = '/SingleMuon/Run2015B-PromptReco-v1/MINIAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 20
config.Data.lumiMask = 'http://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-251252_13TeV_PromptReco_Collisions15_JSON.txt'
config.Data.runRange = '251244-251252' 
#config.Data.outLFNDirBase = '/store/user/%s/' % (getUsernameFromSiteDB())
config.Data.publication = False
config.Data.publishDataName = 'FirstDataSingleMu'

config.Site.storageSite = 'T2_DE_DESY'