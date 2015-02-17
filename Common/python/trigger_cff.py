import FWCore.ParameterSet.Config as cms

Trigger = cms.EDFilter("HLTHighLevel",
				  HLTPaths = cms.vstring("HLT_Iso24*", "HLT_Mu40*", "HLT_Ele95_CaloIdVT_GsfTrkIdT*","HLT_IsoTkMu24*", "HLT_Ele27_eta2p1_WP85_Gsf_v1"),
				  throw = cms.bool(False),
				  TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
				  andOr = cms.bool(True), eventSetupPathsKey = cms.string("")#false = and-mode (all requested triggers), true = or-mode (at least one) 
				 )

TriggerSequence = cms.Sequence(Trigger)