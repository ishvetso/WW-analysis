import FWCore.ParameterSet.Config as cms

goodMuons = cms.EDFilter("CandViewSelector",
                         src = cms.InputTag("TightMuons"),
                         cut = cms.string("pt > 40")
                         )


muSequence = cms.Sequence(goodMuons )
