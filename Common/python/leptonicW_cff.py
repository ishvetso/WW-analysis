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

leptonicVSequence = cms.Sequence(Wtomunu + Wtoenu + leptonicV)
#pt > 200 & (70.0 < mass < 90.0)