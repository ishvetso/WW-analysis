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
                         cut = cms.string("pt > 200")
                         )


Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("pt > 200")
                        )


leptonicWtomunuSequence = cms.Sequence(bestMuon + Wtomunu )
leptonicWtoenuSequence  = cms.Sequence(bestElectron + Wtoenu)

