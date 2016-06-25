import FWCore.ParameterSet.Config as cms

TriggerMuon = cms.EDFilter("HLTHighLevel",
				  HLTPaths = cms.vstring("HLT_Mu50_v*"),
				  throw = cms.bool(False),
				  TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
				  andOr = cms.bool(True), eventSetupPathsKey = cms.string("")#false = and-mode (all requested triggers), true = or-mode (at least one) 
				 )

# electron trigger is disables
TriggerElectron = cms.EDFilter("HLTHighLevel",
				  HLTPaths = cms.vstring("HLT_Ele27_WPLoose_Gsf_v*"),
				  throw = cms.bool(False),
				  TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
				  andOr = cms.bool(True), eventSetupPathsKey = cms.string("")#false = and-mode (all requested triggers), true = or-mode (at least one) 
				 )

