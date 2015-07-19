from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'WW-analysis_mu'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/analysis_mu_MC.py'

config.section_("Data")
config.Data.inputDataset = '/WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
#config.Data.outLFN = '/store/user/<subdir>' # or '/store/group/<subdir>'
config.Data.publication = False
#config.Data.Data.publishDBS = 'phys03'
config.Data.publishDataName = 'WW-analysis_mu'

config.section_("Site")
config.Site.storageSite = 'T2_DE_DESY'
