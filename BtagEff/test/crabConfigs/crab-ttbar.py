from WMCore.Configuration import Configuration
config = Configuration()
config.section_("General")
config.General.requestName = 'ttbar_btag_eff'
config.General.workArea = 'crab_projects'
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '/afs/cern.ch/work/i/ishvetso/aTGCRun2/CMSSW_7_6_3_patch1/src/aTGCsAnalysis/BtagEff/test/btageff.py'
config.section_("Data")
config.Data.inputDataset = '/TTJets_13TeV-amcatnloFXFX-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 2
config.Data.publication = False
config.section_("Site")
config.Site.storageSite = 'T2_DE_DESY'
