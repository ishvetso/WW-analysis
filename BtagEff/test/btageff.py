import FWCore.ParameterSet.Config as cms

process = cms.Process( "BtagEff" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.allowUnscheduled = cms.untracked.bool(False) 

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("aTGCsAnalysis.Common.goodJets_cff")
process.load("aTGCsAnalysis.Common.goodElectrons_cff")
process.load("aTGCsAnalysis.Common.goodMuons_cff")

process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_RunIIFall15DR76_v1'

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


process.muons = cms.Sequence(process.HighPtMuons + process.tightMuons)
process.electrons = cms.Sequence(process.electronIDs + process.tightElectrons)
process.AK8Jets = cms.Sequence(process.selectedPatJetsAK8ByPt + process.selectedPatJetsAK8 + process.cleanJets + process.goodJets + process.bestJet)

process.BtagAnalyzer = cms.EDAnalyzer("BTaggingEffAnalyzer",
									  JetsTag = cms.InputTag("goodAK4Jets"),
									  DiscriminatorTag = cms.string("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
									  DiscriminatorValue = cms.double(0.890),
									  PtNBins = cms.int32(10),
									  PtMin = cms.double(30.),
									  PtMax = cms.double(530.),
									  EtaNBins = cms.int32(5),
									  EtaMin = cms.double(-2.4),
									  EtaMax = cms.double(2.4)
									)

process.btagPath = cms.Path(process.egmGsfElectronIDSequence + process.muons + process.electrons + process.AK8Jets + process.AK4JetsSequence + process.BtagAnalyzer)


process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/mc/RunIIFall15MiniAODv2/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/008262ED-4CB8-E511-BDF3-D067E5F90F2A.root'),
    
)
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree.root")
                                  )
