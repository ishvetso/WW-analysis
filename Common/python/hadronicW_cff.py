import FWCore.ParameterSet.Config as cms


hadronicV = cms.EDFilter("PATJetSelector",
                         src = cms.InputTag("goodJets"),
                         cut = cms.string('')
                        )

hadronicVSequence = cms.Sequence(hadronicV)
