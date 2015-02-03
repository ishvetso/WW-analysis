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

### Hadronic and leptonic boson.
### Naturally, you should choose the one channel you need
process.load("WW-analysis.Common.leptonicW_cff")
process.load("WW-analysis.Common.hadronicW_cff")

# module to print out generator information (not included in the path)
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.printTree = cms.EDAnalyzer("ParticleListDrawer",
  maxEventsToPrint = cms.untracked.int32(1000),
  printVertex = cms.untracked.bool(False),
  src = cms.InputTag("prunedGenParticles")
)


# Muons
process.TightMuons = cms.EDProducer("TightMuonSelector",
                         src = cms.InputTag("slimmedMuons"),
                         vertex = cms.InputTag("offlineSlimmedPrimaryVertices")
                         )

# Electrons

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")

process.GlobalTag.globaltag = 'PHYS14_25_V1'
#
# START ELECTRON ID SECTION
#
# Set up everything that is needed to compute electron IDs and
# add the ValueMaps with ID decisions into the event data stream
#

# Load tools and function definitions
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *

process.load("RecoEgamma.ElectronIdentification.egmGsfElectronIDs_cfi")
# overwrite a default parameter: for miniAOD, the collection name is a slimmed one
process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag('slimmedElectrons')

from PhysicsTools.SelectorUtils.centralIDRegistry import central_id_registry
process.egmGsfElectronIDSequence = cms.Sequence(process.egmGsfElectronIDs)

# Define which IDs we want to produce
# Each of these two example IDs contains all four standard 
# cut-based ID working points (only two WP of the PU20bx25 are actually used here).
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V0_miniAOD_cff']
#Add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

# Do not forget to add the egmGsfElectronIDSequence to the path,
# as in the example below!

#
# END ELECTRON ID SECTION



process.leptonicVFilter = cms.EDFilter("CandViewCountFilter",
                                       src = cms.InputTag("leptonicV"),
                                       minNumber = cms.uint32(1),
                                      )

process.hadronicVFilter = cms.EDFilter("CandViewCountFilter",
                                       src = cms.InputTag("hadronicV"),
                                       minNumber = cms.uint32(1),
                                      )



process.leptonSequence = cms.Sequence(process.muSequence +
                                      process.electronIDs +
                                      process.goodElectrons +
                                      process.leptonicVSequence +
                                      process.leptonicVFilter )


process.jetSequence = cms.Sequence(process.JetsWithBTagSequence +
				   process.fatJetsSequence +
                                   process.hadronicV +
                                   process.hadronicVFilter)


print "++++++++++ CUTS ++++++++++\n"
print "Leptonic V cut = "+str(process.leptonicV.cut)
print "Hadronic V cut = "+str(process.hadronicV.cut)
print "\n++++++++++++++++++++++++++"

process.treeDumper = cms.EDAnalyzer("TreeMaker",
                                    hadronicVSrc = cms.string("hadronicV"),
                                    leptonicVSrc = cms.string("leptonicV"),
                                    metSrc = cms.string("slimmedMETs"),
                                    genSrc = cms.string("prunedGenParticles"),
                                    jetSrc = cms.string("jetsWithTau"),
                                    jets_btag_veto_Src  = cms.string("cleanJetsWithBTag"),
                                    vertex_Src = cms.string("offlineSlimmedPrimaryVertices"),
                                    )


process.DecayChannel = cms.EDAnalyzer("DecayChannelAnalyzer")


process.analysis = cms.Path(process.DecayChannel +  process.egmGsfElectronIDSequence  +   process.TightMuons + process.leptonSequence +   process.jetSequence +  process.treeDumper )
process.maxEvents.input = 1000
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_PHYS14.root')
    
)

#file:///afs/cern.ch/work/i/ishvetso/RunII_preparation/samples/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_PHYS14.root'


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.FwkReport.limit = 99999999

'''process.out = cms.OutputModule("PoolOutputModule",
 fileName = cms.untracked.string('patTuple.root'),
  outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)
'''
process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree.root")
                                  )