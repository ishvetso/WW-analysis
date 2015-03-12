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
cleanJets.checkOverlaps.muons.src = "tightMuons"
cleanJets.checkOverlaps.muons.deltaR = 1.0
cleanJets.checkOverlaps.muons.requireNoOverlaps = True


#cleanJets.checkOverlaps.muons = cms.PSet()

cleanJets.checkOverlaps.electrons.src = "tightElectrons"
cleanJets.checkOverlaps.electrons.deltaR = 1.0
cleanJets.checkOverlaps.electrons.requireNoOverlaps = True
cleanJets.checkOverlaps.photons = cms.PSet()
cleanJets.checkOverlaps.taus = cms.PSet()
cleanJets.checkOverlaps.tkIsoElectrons = cms.PSet()
cleanJets.finalCut = "pt > 40 & abs(eta) < 2.4"

'''jetsWithTau = cms.EDProducer("NJettinessAdder",
                             src = cms.InputTag("cleanJets"),
                             cone = cms.double(0.8)
                             )'''
# Create a different collection of jets which  contains b-tagging information. This is necessary because slimmedJetsAK8 jets don't contain BTagInfo

AK4Jets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("slimmedJets")
                        )

cleanAK4Jets = jetCleaner_cfi.cleanPatJets.clone()
cleanAK4Jets.src = "AK4Jets"
cleanAK4Jets.checkOverlaps.muons.src = "tightMuons"
cleanAK4Jets.checkOverlaps.muons.deltaR = 0.3
cleanAK4Jets.checkOverlaps.muons.requireNoOverlaps = True


#cleanJets.checkOverlaps.muons = cms.PSet()

cleanAK4Jets.checkOverlaps.electrons.src = "tightElectrons"
cleanAK4Jets.checkOverlaps.electrons.deltaR = 0.8
cleanAK4Jets.checkOverlaps.electrons.requireNoOverlaps = True
cleanAK4Jets.checkOverlaps.photons = cms.PSet()
cleanAK4Jets.checkOverlaps.taus = cms.PSet()
cleanAK4Jets.checkOverlaps.tkIsoElectrons = cms.PSet()
cleanAK4Jets.finalCut = "pt > 30 & abs(eta) < 2.4"

fatJetsSequence = cms.Sequence(goodJets + cleanJets )
AK4JetsSequence = cms.Sequence(AK4Jets + cleanAK4Jets)
