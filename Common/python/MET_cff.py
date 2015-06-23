import FWCore.ParameterSet.Config as cms

METele = cms.EDFilter("PATMETSelector",
                         src = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 80")
                         )

METmu = cms.EDFilter("PATMETSelector",
                         src = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 40")
                         )


