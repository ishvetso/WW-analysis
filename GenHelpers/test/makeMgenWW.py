import FWCore.ParameterSet.Config as cms
process = cms.Process( "makeWWmass" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.allowUnscheduled = cms.untracked.bool(False) 

process.makeWWmass = cms.EDAnalyzer("MakeMWWGenDist",
									genInfo = cms.InputTag("generator"),
									LHEEventProductSource = cms.InputTag("source"),
									genSource = cms.InputTag("prunedGenParticles"),
									isSignal = cms.bool(False)
									)

# PATH
process.analysis = cms.Path( process.makeWWmass)


#process.maxEvents.input = 1000
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/mc/RunIISpring15MiniAODv2/WWToLNuQQ_aTGC_13TeV-madgraph-pythia8/MINIAODSIM/Asympt25ns_74X_mcRun2_asymptotic_v2-v1/20000/0E23000B-20B2-E511-84BC-78E7D1216D38.root'),
)



process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1


process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("M_WW_gen.root")
                                  )