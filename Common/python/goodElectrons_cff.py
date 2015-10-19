import FWCore.ParameterSet.Config as cms

electronIDs = cms.EDFilter("ElectronID",
			   ValueMap_src = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
                           electron_src = cms.InputTag("slimmedElectrons"),
			    #False -> runs as producer 
			    #True -> filters events
			    filter_flag = cms.bool(False),
			   )

looseElectrons = cms.EDFilter("PATElectronSelector",
                         src = cms.InputTag("electronIDs"),
                         cut = cms.string("pt > 35"),
                         filter = cms.bool(False)
                         )

tightElectrons = cms.EDFilter("PATElectronSelector",
                         src = cms.InputTag("electronIDs"),
                         cut = cms.string("pt > 140"),
                         filter = cms.bool(False)
                         )

eleSequence = cms.Sequence(electronIDs + looseElectrons + tightElectrons)
