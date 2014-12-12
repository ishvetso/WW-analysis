import FWCore.ParameterSet.Config as cms

electronIDs = cms.EDFilter("ElectronID",
			   ValueMap_src = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V0-miniAOD-standalone-medium"),
                           electron_src = cms.InputTag("slimmedElectrons"),
			    #False -> runs as producer 
			    #True -> filters events
			    filter_flag = cms.bool(False),
			   )

goodElectrons = cms.EDFilter("CandViewSelector",
                         src = cms.InputTag("electronIDs"),
                         cut = cms.string("pt > 40")
                         )