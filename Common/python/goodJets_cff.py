import FWCore.ParameterSet.Config as cms
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector

goodJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("slimmedJetsAK8")
                        )

### Cleaning
# We want to make sure that the jets are not the electrons or muons done previously

import PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi as jetCleaner_cfi

cleanJets = jetCleaner_cfi.cleanPatJets.clone()
cleanJets.src = "goodJets"
cleanJets.checkOverlaps.muons.src = "goodMuons"
cleanJets.checkOverlaps.muons.deltaR = 0.8
cleanJets.checkOverlaps.muons.requireNoOverlaps = True


#cleanJets.checkOverlaps.muons = cms.PSet()

cleanJets.checkOverlaps.electrons.src = "goodElectrons"
cleanJets.checkOverlaps.electrons.deltaR = 0.8
cleanJets.checkOverlaps.electrons.requireNoOverlaps = True
cleanJets.checkOverlaps.photons = cms.PSet()
cleanJets.checkOverlaps.taus = cms.PSet()
cleanJets.checkOverlaps.tkIsoElectrons = cms.PSet()
cleanJets.finalCut = "pt > 40 & abs(eta) < 2.4"

jetsWithTau = cms.EDProducer("NJettinessAdder",
                             src = cms.InputTag("cleanJets"),
                             cone = cms.double(0.8)
                             )
# Create a different collection of jets which  contains b-tagging information. This is necessary because slimmedJetsAK8 jets don't contain BTagInfo

JetsWithBTag = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("slimmedJets")
                        )

cleanJetsWithBTag = jetCleaner_cfi.cleanPatJets.clone()
cleanJetsWithBTag.src = "JetsWithBTag"
cleanJetsWithBTag.checkOverlaps.muons.src = "goodMuons"
cleanJetsWithBTag.checkOverlaps.muons.deltaR = 0.5
cleanJetsWithBTag.checkOverlaps.muons.requireNoOverlaps = True


#cleanJets.checkOverlaps.muons = cms.PSet()

cleanJetsWithBTag.checkOverlaps.electrons.src = "goodElectrons"
cleanJetsWithBTag.checkOverlaps.electrons.deltaR = 0.8
cleanJetsWithBTag.checkOverlaps.electrons.requireNoOverlaps = True
cleanJetsWithBTag.checkOverlaps.photons = cms.PSet()
cleanJetsWithBTag.checkOverlaps.taus = cms.PSet()
cleanJetsWithBTag.checkOverlaps.tkIsoElectrons = cms.PSet()
cleanJetsWithBTag.finalCut = "pt > 30 & abs(eta) < 2.4"

fatJetsSequence = cms.Sequence(goodJets + cleanJets + jetsWithTau )
JetsWithBTagSequence = cms.Sequence(JetsWithBTag + cleanJetsWithBTag)
