from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'WW-analysis'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_2_2_patch1/src/WW-analysis/Common/analysis.py'

config.section_("Data")
config.Data.inputDataset = '/RSGravitonToWW_kMpl01_M_2000_Tune4C_13TeV_pythia8/Phys14DR-PU20bx25_PHYS14_25_V1-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
#config.Data.outLFN = '/store/user/<subdir>' # or '/store/group/<subdir>'
config.Data.publication = True
#config.Data.Data.publishDBS = 'phys03'
config.Data.publishDataName = 'WW-analysis'

config.section_("Site")
config.Site.storageSite = 'T2_DE_DESY'
