import FWCore.ParameterSet.Config as cms

process = cms.Process( "WWanalysis" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.allowUnscheduled = cms.untracked.bool(False) 

process.load("WW-analysis.Common.goodMuons_cff")
process.load("WW-analysis.Common.goodElectrons_cff")
process.load("WW-analysis.Common.goodJets_cff")
process.load("WW-analysis.Common.trigger_cff")

### Hadronic and leptonic boson.
### Naturally, you should choose the one channel you need
process.load("WW-analysis.Common.leptonicW_cff")
process.load("WW-analysis.Common.hadronicW_cff")

# Electrons

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'PHYS14_25_V2::All'

# START ELECTRON ID SECTION
#
# Set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream
#
# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')
from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)
#add in the heep ID to the producer. You can run with other IDs but heep ID must be loaded with setupVIDSelection, not setupAllVIDSelection as heep works differently because mini-aod and aod are defined in the same file to ensure consistancy (you cant change cuts of aod without changing miniaod
process.load('RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV51_cff')
setupVIDSelection(process.egmGsfElectronIDs,process.heepElectronID_HEEPV51_miniAOD)
# Do not forget to add the egmGsfElectronIDSequence to the path,
# as in the example below!
#

#process.electronIDs.ValueMap_src = cms.InputTag("egmGsfElectronsIDs:heepElectronID-HEEPV51-miniAOD")

# END ELECTRON ID SECTION

process.leptonFilter = cms.EDFilter("CandViewCountFilter",
				    src = cms.InputTag("slimmedMuons"),
                                    minNumber = cms.uint32(1),
				   )

process.leptonSequence = cms.Sequence(process.leptonFilter +
				      process.muSequence +
				      process.eleSequence +
                                      process.Wtomunu )

process.jetFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("slimmedJetsAK8"),
                                 minNumber = cms.uint32(1),
                                )

process.jetSequence = cms.Sequence(process.jetFilter +
				   process.substructureSequence +
				   process.redoPatJets + 
				   process.fatJetsSequence +
				   process.AK4JetsSequence +
				   process.hadronicV)


process.treeDumper = cms.EDAnalyzer("TreeMaker",
                                    hadronicVSrc = cms.string("hadronicV"),
                                    leptonicVSrc = cms.string("Wtomunu"),
                                    metSrc = cms.string("slimmedMETs"),
                                    genSrc = cms.string("prunedGenParticles"),
                                    jetSrc = cms.string("goodJets"),
                                    jets_btag_veto_Src  = cms.string("cleanAK4Jets"),
                                    vertex_Src = cms.string("offlineSlimmedPrimaryVertices"),
                                    looseEleSrc = cms.string("looseElectrons"),
                                    looseMuSrc = cms.string("looseMuons"),
                                    leptonSrc = cms.string("tightMuons"),
                                    )


# Identify the channel 
process.DecayChannel = cms.EDAnalyzer("DecayChannelAnalyzer")

# PATH
process.analysis = cms.Path(process.DecayChannel +  process.egmGsfElectronIDSequence +  process.leptonSequence +   process.jetSequence +  process.treeDumper)


process.maxEvents.input = 1000
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_PHYS14.root')
    
)

#file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_PHYS14.root'


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.FwkReport.limit = 99999999

process.out = cms.OutputModule("PoolOutputModule",
 fileName = cms.untracked.string('patTuple.root'),
  outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)

process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree.root")
                                  )
