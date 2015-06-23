import FWCore.ParameterSet.Config as cms


hadronicV = cms.EDFilter("PATJetSelector",
                         src = cms.InputTag("goodJets"),
                         cut = cms.string('userFloat("ak8PFJetsCHSPrunedMass") > 40.0 & userFloat("ak8PFJetsCHSPrunedMass") < 130.0')
                        )

hadronicVSequence = cms.Sequence(hadronicV)
