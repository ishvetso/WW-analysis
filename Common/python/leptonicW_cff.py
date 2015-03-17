import FWCore.ParameterSet.Config as cms

Wtomunu = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("looseMuons"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("")
                         )


Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("looseElectrons"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("")
                        )

leptonicV = cms.EDProducer("CandViewMerger",
                           src = cms.VInputTag( "Wtoenu", "Wtomunu"),
                           cut = cms.string("")
                           ) 

