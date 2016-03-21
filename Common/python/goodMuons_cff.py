import FWCore.ParameterSet.Config as cms

HighPtMuons = cms.EDProducer("HighPtMuonSelector",
                         src = cms.InputTag("slimmedMuons"),
                         vertex = cms.InputTag("offlineSlimmedPrimaryVertices")
                         )

looseMuons = cms.EDFilter("PATMuonSelector",
                         src = cms.InputTag("HighPtMuons"),
                         cut = cms.string("tunePMuonBestTrack().pt > 20 & trackIso/(tunePMuonBestTrack().pt) < 0.1 & abs(tunePMuonBestTrack().eta) < 2.4"),
                         filter = cms.bool(False)
                         )

tightMuons = cms.EDFilter("PATMuonSelector",
                         src = cms.InputTag("HighPtMuons"),
                         cut = cms.string("tunePMuonBestTrack().pt > 50 & trackIso/(tunePMuonBestTrack().pt) < 0.1 & abs(tunePMuonBestTrack().eta) < 2.1"),
                         filter = cms.bool(False)
                         )


muSequence = cms.Sequence(HighPtMuons + looseMuons + tightMuons)
