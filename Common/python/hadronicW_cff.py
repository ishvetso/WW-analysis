import FWCore.ParameterSet.Config as cms


hadronicV = cms.EDFilter("PATJetSelector",
                         src = cms.InputTag("cleanJets"),
                           cut = cms.string('pt > 40 & ' + \
			   '(userFloat("ak8PFJetsCHSPrunedLinks") > 60.0) &'+\
			   '(userFloat("ak8PFJetsCHSPrunedLinks") < 100.0)')
                         )

hadronicVSequence = cms.Sequence(hadronicV)
