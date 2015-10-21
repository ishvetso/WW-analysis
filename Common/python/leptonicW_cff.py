import FWCore.ParameterSet.Config as cms

bestMuon =cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("tightMuons"), 
    maxNumber = cms.uint32(1)
  )

bestMuonEnUp =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestMuon"), 
    channel = cms.string("mu"),
    variation = cms.string("up")
  )

bestMuonEnDown =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestMuon"), 
    channel = cms.string("mu"),
    variation = cms.string("down")
  )


bestElectronEnUp =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestElectron"), 
    channel = cms.string("el"),
    variation = cms.string("up")
  )

bestElectronEnDown =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestElectron"), 
    channel = cms.string("el"),
    variation = cms.string("down")
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


leptonicWtomunuSequence = cms.Sequence(bestMuon + bestMuonEnUp+ bestMuonEnDown + Wtomunu )
leptonicWtoenuSequence  = cms.Sequence(bestElectron + bestElectronEnUp + bestElectronEnDown + Wtoenu)

