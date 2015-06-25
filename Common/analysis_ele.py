import FWCore.ParameterSet.Config as cms

process = cms.Process( "WWanalysis" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.allowUnscheduled = cms.untracked.bool(False) 

process.load("WW-analysis.Common.goodMuons_cff")
process.load("WW-analysis.Common.goodElectrons_cff")
process.load("WW-analysis.Common.MET_cff")
process.load("WW-analysis.Common.goodJets_cff")
process.load("WW-analysis.Common.trigger_cff")

### Hadronic and leptonic boson.
### Naturally, you should choose the one channel you need
process.load("WW-analysis.Common.leptonicW_cff")
process.load("WW-analysis.Common.hadronicW_cff")

# Electrons

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.GlobalTag.globaltag = 'MCRUN2_74_V9::All'

#
# Set up electron ID (VID framework)
#

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 
dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV51_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

#
# Configure an example module for user analysis with electrons
#

process.leptonFilter = cms.EDFilter("LeptonVeto",
				    looseLeptonSrc = cms.InputTag("looseElectrons"),
				    tightLeptonSrc = cms.InputTag("tightElectrons"),
                                    minNLoose = cms.int32(1),
                                    maxNLoose = cms.int32(1),
                                    minNTight = cms.int32(1),
                                    maxNTight = cms.int32(1),
				   )

process.leptonSequence = cms.Sequence(process.muSequence +
				      process.eleSequence +
				      process.leptonFilter +
                                      process.leptonicWtoenuSequence )

process.jetFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("slimmedJetsAK8"),
                                 minNumber = cms.uint32(1),
                                )

process.jetSequence = cms.Sequence(process.jetFilter +
				   process.fatJetsSequence +
				   process.AK4JetsSequence +
				   process.hadronicV)


process.treeDumper = cms.EDAnalyzer("TreeMaker",
                                    hadronicVSrc = cms.string("hadronicV"),
                                    leptonicVSrc = cms.string("Wtoenu"),
                                    metSrc = cms.InputTag("METele"),
                                    genSrc = cms.string("prunedGenParticles"),
                                    jetSrc = cms.string("goodJets"),
                                    jets_btag_veto_Src  = cms.string("goodAK4Jets"),
                                    vertex_Src = cms.string("offlineSlimmedPrimaryVertices"),
                                    looseEleSrc = cms.string("looseElectrons"),
                                    looseMuSrc = cms.string("looseMuons"),
                                    leptonSrc = cms.string("tightElectrons"),
                                    )


# Identify the channel 
process.DecayChannel = cms.EDAnalyzer("DecayChannelAnalyzer")

# PATH
process.analysis = cms.Path(process.DecayChannel + process.METele +  process.egmGsfElectronIDSequence +  process.leptonSequence +   process.jetSequence  + process.treeDumper)


#process.maxEvents.input = 1000
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/WW_74X.root')
    
)

#file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_PHYS14.root'


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.FwkReport.limit = 99999999

process.out = cms.OutputModule("PoolOutputModule",
 fileName = cms.untracked.string('patTuple.root'),
  outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)

process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree_ele.root")
                                  )
