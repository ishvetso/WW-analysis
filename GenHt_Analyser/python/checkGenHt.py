import FWCore.ParameterSet.Config as cms

process = cms.Process( "CheckGenHtWJets" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

process.CheckHt_Analyser = cms.EDAnalyzer("GenHt_Analyser")

process.analysis = cms.Path(process.CheckHt_Analyser)
process.maxEvents.input = 100
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring()
    
)

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100
#process.MessageLogger.cerr.FwkReport.limit = 99999999

process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree.root")
                                  )