import FWCore.ParameterSet.Config as cms

TriggerMuon = cms.EDFilter("HLTHighLevel",
				  HLTPaths = cms.vstring("HLT_Mu45_eta2p1_v*"),
				  throw = cms.bool(False),
				  TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
				  andOr = cms.bool(True), eventSetupPathsKey = cms.string("")#false = and-mode (all requested triggers), true = or-mode (at least one) 
				 )

TriggerElectron = cms.EDFilter("HLTHighLevel",
				  HLTPaths = cms.vstring("HLT_Ele105_CaloIdVT_GsfTrkIdT_v*"),
				  throw = cms.bool(False),
				  TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
				  andOr = cms.bool(True), eventSetupPathsKey = cms.string("")#false = and-mode (all requested triggers), true = or-mode (at least one) 
				 )

