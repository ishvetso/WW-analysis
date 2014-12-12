import FWCore.ParameterSet.Config as cms

#Wtomunu = cms.EDProducer("CandViewShallowCloneCombiner",
#                         decay = cms.string("goodMuons genMetTrue"),
#                         checkCharge = cms.bool(False),
#                         cut = cms.string("mt > 50 & pt > 80")
#                         )

#Wtoenu = cms.EDProducer("CandViewCombiner",
#                        decay = cms.string("goodElectrons genMetTrue"),
#                        checkCharge = cms.bool(False),
#                        cut = cms.string("mt > 50 & pt > 80")
#                        )

Wtomunu = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("goodMuons"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 100")
                         )


Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("goodElectrons"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("pt > 100")
                        )

leptonicV = cms.EDProducer("CandViewMerger",
                           src = cms.VInputTag( "Wtoenu", "Wtomunu"),
                           cut = cms.string("")
                           ) 

leptonicVSequence = cms.Sequence(Wtomunu + Wtoenu + leptonicV)
#pt > 200 & (70.0 < mass < 90.0)