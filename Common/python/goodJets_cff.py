import FWCore.ParameterSet.Config as cms
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from RecoJets.JetProducers.ak4PFJets_cfi import *

# adding softdrop



import PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi as jetCleaner_cfi

cleanJets = jetCleaner_cfi.cleanPatJets.clone()
cleanJets.src = "slimmedJetsAK8"
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
cleanJets.finalCut = ""


goodJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("cleanJets")
                        )


bestJet =cms.EDFilter("LargestPtCandViewSelector",
    src = cms.InputTag("goodJets"), 
    maxNumber = cms.uint32(1)
  )

fatJetsSequence = cms.Sequence(cleanJets + goodJets + bestJet)
# Create a different collection of jets which  contains b-tagging information. This is necessary because slimmedJetsAK8 jets don't contain BTagInfo


cleanAK4Jets = jetCleaner_cfi.cleanPatJets.clone()
cleanAK4Jets.src = "slimmedJets"
cleanAK4Jets.checkOverlaps.muons.src = "tightMuons"
cleanAK4Jets.checkOverlaps.muons.deltaR = 0.3
cleanAK4Jets.checkOverlaps.muons.requireNoOverlaps = True


#cleanJets.checkOverlaps.muons = cms.PSet()

cleanAK4Jets.checkOverlaps.electrons.src = "tightElectrons"
cleanAK4Jets.checkOverlaps.electrons.deltaR = 0.3
cleanAK4Jets.checkOverlaps.electrons.requireNoOverlaps = True
cleanAK4Jets.checkOverlaps.photons = cms.PSet()
cleanAK4Jets.checkOverlaps.taus = cms.PSet()
cleanAK4Jets.checkOverlaps.tkIsoElectrons = cms.PSet()
cleanAK4Jets.checkOverlaps.jets = cms.PSet(
					src = cms.InputTag("bestJet"),
					algorithm = cms.string("byDeltaR"),
					preselection = cms.string(""),
					deltaR = cms.double(0.8),
					checkRecoComponents = cms.bool(False), # don't check if they share some AOD object ref
					pairCut = cms.string(""),
					requireNoOverlaps = cms.bool(True), # overlaps don't cause the jet to be discared
				      )

cleanAK4Jets.finalCut = "pt > 30 & abs(eta) < 2.4"

goodAK4Jets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("cleanAK4Jets")
                        )



AK4JetsSequence = cms.Sequence(cleanAK4Jets + goodAK4Jets)

