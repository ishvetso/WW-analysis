import FWCore.ParameterSet.Config as cms

HighPtMuons = cms.EDProducer("HighPtMuonSelector",
                         src = cms.InputTag("slimmedMuons"),
                         vertex = cms.InputTag("offlineSlimmedPrimaryVertices")
                         )
looseMuons = cms.EDFilter("CandViewSelector",
                         src = cms.InputTag("HighPtMuons"),
                         cut = cms.string("pt > 20 & trackIso/pt < 0.1 & abs(eta) < 2.4")
                         )

tightMuons = cms.EDFilter("CandViewSelector",
                         src = cms.InputTag("HighPtMuons"),
                         cut = cms.string("pt > 40 & trackIso/pt < 0.1 & abs(eta) < 2.4")
                         )


muSequence = cms.Sequence(HighPtMuons + looseMuons + tightMuons)
