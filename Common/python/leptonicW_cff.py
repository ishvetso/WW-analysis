import FWCore.ParameterSet.Config as cms

bestMuon =cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("tightMuons"), 
    maxNumber = cms.uint32(1)
  )

bestElectron =cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("tightElectrons"), 
    maxNumber = cms.uint32(1)
  )

Wtomunu = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("bestMuon"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("")
                         )


Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("")
                        )

leptonicV = cms.EDProducer("CandViewMerger",
                           src = cms.VInputTag( "Wtoenu", "Wtomunu"),
                           cut = cms.string("")
                           ) 

leptonicVSequence = cms.Sequence(bestElectron + bestMuon + Wtomunu + Wtoenu + leptonicV)

