from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'WZ_mu_$FEATURE$'
config.General.workArea = 'crab_projects'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '$configDIR$analysis_mu_MC.py'

config.section_("Data")
config.Data.inputDataset = '/WZTo1L1Nu2Q_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.section_("Site")
config.Site.storageSite = 'T2_DE_DESY'
