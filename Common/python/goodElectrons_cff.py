import FWCore.ParameterSet.Config as cms

electronIDs = cms.EDFilter("ElectronID",
			   ValueMap_src = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
                           electron_src = cms.InputTag("slimmedElectrons"),
			    #False -> runs as producer 
			    #True -> filters events
			    filter_flag = cms.bool(False),
			   )

looseElectrons = cms.EDFilter("CandViewSelector",
                         src = cms.InputTag("electronIDs"),
                         cut = cms.string("pt > 35")
                         )

tightElectrons = cms.EDFilter("CandViewSelector",
                         src = cms.InputTag("electronIDs"),
                         cut = cms.string("pt > 90")
                         )

eleSequence = cms.Sequence(electronIDs + looseElectrons + tightElectrons)
