import FWCore.ParameterSet.Config as cms

METele = cms.EDFilter("PATMETSelector",
                         src = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 40"),
                         filter = cms.bool(True)
                         )

METmu = cms.EDFilter("PATMETSelector",
                         src = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 35"),
                         filter = cms.bool(True)
                         )


