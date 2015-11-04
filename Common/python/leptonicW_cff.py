import FWCore.ParameterSet.Config as cms

bestMuon =cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("tightMuons"), 
    maxNumber = cms.uint32(1)
  )
bestElectron =cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("tightElectrons"), 
    maxNumber = cms.uint32(1)
  )

# lepton energy scale uncertainties

bestMuonEnUp =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestMuon"), 
    channel = cms.string("mu"),
    type = cms.string("scale"),
    variation = cms.string("up")
  )

bestMuonEnDown =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestMuon"), 
    channel = cms.string("mu"),
    type = cms.string("scale"),
    variation = cms.string("down")
  )


bestElectronEnUp =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestElectron"), 
    channel = cms.string("el"),
    type = cms.string("scale"),
    variation = cms.string("up")
  )

bestElectronEnDown =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestElectron"), 
    channel = cms.string("el"),
    type = cms.string("scale"),
    variation = cms.string("down"),
  )

#resolution uncertainties 

bestElectronResUp =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("tightElectrons"), 
    channel = cms.string("el"),
    type = cms.string("resolution"),
    variation = cms.string("up"),
    genSource = cms.InputTag("prunedGenParticles"),
  )

bestElectronResDown =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("tightElectrons"), 
    channel = cms.string("el"),
    type = cms.string("resolution"),
    variation = cms.string("down"),
    genSource = cms.InputTag("prunedGenParticles"),
  )

bestMuonResUp =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestMuon"), 
    channel = cms.string("mu"),
    type = cms.string("resolution"),
    variation = cms.string("up"),
    genSource = cms.InputTag("prunedGenParticles"),
  )

bestMuonResDown =cms.EDProducer("LeptonSystematicsProducer",
    leptonSource = cms.InputTag("bestMuon"), 
    channel = cms.string("mu"),
    type = cms.string("resolution"),
    variation = cms.string("down"),
    genSource = cms.InputTag("prunedGenParticles"),
  )

Wtomunu = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("bestMuon"),
                         MET = cms.InputTag("METmu"),
                         cut = cms.string("")
                         )


Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("METele"),
                        cut = cms.string("")
                        )


leptonicWtomunuSequenceMC = cms.Sequence( bestMuon + bestMuonEnUp+ bestMuonEnDown + bestMuonResUp + bestMuonResDown + Wtomunu )
leptonicWtoenuSequenceMC  = cms.Sequence(bestElectron + bestElectronEnUp + bestElectronEnDown + bestElectronResUp + bestElectronResDown +  Wtoenu)

leptonicWtomunuSequenceData = cms.Sequence(bestMuon + Wtomunu)
leptonicWtoenuSequenceData  = cms.Sequence(bestElectron + Wtoenu)

