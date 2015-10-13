import FWCore.ParameterSet.Config as cms

process = cms.Process( "WWanalysis" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
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

##___________________________HCAL_Noise_Filter________________________________||
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)

process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
   reverseDecision = cms.bool(False)
)

process.NoiseFilters = cms.EDFilter("NoiseFilter",
            noiseFilter = cms.InputTag("TriggerResults", "", "HLT"),
            filterNames = cms.vstring("Flag_CSCTightHaloFilter", "Flag_goodVertices ")  )


#
# Set up electron ID (VID framework)
#

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 
dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

#
# Configure an example module for user analysis with electrons
#

process.ElectronVeto = cms.EDFilter("LeptonVeto",
				    looseLeptonSrc = cms.InputTag("looseElectrons"),
				    tightLeptonSrc = cms.InputTag("tightElectrons"),
                                    minNLoose = cms.int32(1),
                                    maxNLoose = cms.int32(1),
                                    minNTight = cms.int32(1),
                                    maxNTight = cms.int32(1),
				   )

process.MuonVeto = cms.EDFilter("LeptonVeto",
            looseLeptonSrc = cms.InputTag("looseMuons"),
            tightLeptonSrc = cms.InputTag("tightMuons"),
                                    minNLoose = cms.int32(0),
                                    maxNLoose = cms.int32(0),
                                    minNTight = cms.int32(0),
                                    maxNTight = cms.int32(0),
           )

process.leptonSequence = cms.Sequence(process.muSequence + process.eleSequence + process.ElectronVeto + process.MuonVeto +  process.leptonicWtoenuSequence )

process.jetFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("goodJets"),
                                 minNumber = cms.uint32(1),
                                )

process.jetSequence = cms.Sequence(process.fatJetsSequence +
				                            process.jetFilter+
                          				   process.AK4JetsSequence +
                          				   process.hadronicV)


process.treeDumper = cms.EDAnalyzer("TreeMaker",
                                    PUInfo = cms.InputTag("addPileupInfo"),
                                    filenameData = cms.FileInPath("WW-analysis/PUTrueDistProducer/test/MyDataPileupHistogram.root"),
                                    filenameMC = cms.FileInPath("WW-analysis/PUTrueDistProducer/test/PU_dist.root"),
                                    HistnameData = cms.string("pileup"),
                                    HistnameMC = cms.string("PUTrueDist/pileup"),
                                    hadronicVSrc = cms.InputTag("hadronicV"),
                                    leptonicVSrc = cms.InputTag("Wtoenu"),
                                    metSrc = cms.InputTag("METele"),
                                    genSrc = cms.InputTag("prunedGenParticles"),
                                    fatJetSrc = cms.InputTag("goodJets"),
                                    AK4JetSrc  = cms.InputTag("goodAK4Jets"),
                                    vertexSrc = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                    looseEleSrc = cms.InputTag("looseElectrons"),
                                    looseMuSrc = cms.InputTag("looseMuons"),
                                    leptonSrc = cms.InputTag("tightElectrons"),
                                    isMC = cms.bool(True),
                                    isMuonChannel = cms.bool(False)
                                    )


# Identify the channel 
process.DecayChannel = cms.EDAnalyzer("DecayChannelAnalyzer")

# PATH
process.analysis = cms.Path(process.HBHENoiseFilterResultProducer + process.ApplyBaselineHBHENoiseFilter + process.NoiseFilters  + process.TriggerElectron + process.METele +  process.egmGsfElectronIDSequence +  process.leptonSequence +   process.jetSequence  + process.treeDumper)


#process.maxEvents.input = 1000
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/CMSSW_7_4_7/src/WW-analysis/Common/test/samples_MINIAOD/RSGrav1000.root'),
    #eventsToProcess = cms.untracked.VEventRange('1:75:72317')
    
)

#file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_PHYS14.root'


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
#process.MessageLogger.cerr.FwkReport.limit = 99999999
'''
process.out = cms.OutputModule("PoolOutputModule",
 fileName = cms.untracked.string('patTuple.root'),
  outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)'''

process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree_ele.root")
                                  )
