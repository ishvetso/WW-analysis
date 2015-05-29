import FWCore.ParameterSet.Config as cms
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from RecoJets.JetProducers.ak4PFJets_cfi import *

# adding softdrop

from RecoJets.Configuration.RecoPFJets_cff import ak8PFJetsCHS, ak8PFJetsCHSPruned, ak8PFJetsCHSSoftDrop, ak8PFJetsCHSPrunedLinks, ak8PFJetsCHSSoftDropLinks

chs = cms.EDFilter("CandPtrSelector",
		  src = cms.InputTag('packedPFCandidates'),
		  cut = cms.string('fromPV')
		  )


ak8PFJetsCHS.src = cms.InputTag('chs')
ak8PFJetsCHS.jetPtMin = cms.double(100.0)
ak8PFJetsCHSPruned.src = cms.InputTag('chs')
ak8PFJetsCHSSoftDrop.src = cms.InputTag('chs')



from RecoJets.JetProducers.nJettinessAdder_cfi import Njettiness
Njettiness.src = cms.InputTag('ak8PFJetsCHS')

substructureSequence = cms.Sequence(chs +
				    ak8PFJetsCHS +
				    ak8PFJetsCHSPruned +
				    ak8PFJetsCHSSoftDrop +				    
				    ak8PFJetsCHSPrunedLinks +
				    ak8PFJetsCHSSoftDropLinks +
				    Njettiness)


patJetCorrFactorsAK8 = cms.EDProducer("JetCorrFactorsProducer",
				      src = cms.InputTag("ak8PFJetsCHS"),
				      emf = cms.bool(False),
				      extraJPTOffset = cms.string('L1FastJet'),
				      primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
				      levels = cms.vstring('L1FastJet',
				      'L2Relative',
				      'L3Absolute'),
				      useNPV = cms.bool(True),
				      rho = cms.InputTag("fixedGridRhoFastjetAll"),
				      useRho = cms.bool(True),
				      payload = cms.string('AK8PFchs'),
				      flavorType = cms.string('J')
				      )
patJetsAK8 = cms.EDProducer("PATJetProducer",
			    addJetCharge = cms.bool(False),
			    addGenJetMatch = cms.bool(False),
			    embedGenJetMatch = cms.bool(False),
			    addAssociatedTracks = cms.bool(False),
			    addBTagInfo = cms.bool(False),
			    partonJetSource = cms.InputTag("NOT_IMPLEMENTED"),
			    addGenPartonMatch = cms.bool(False),
			    JetPartonMapSource = cms.InputTag(""),
			    resolutions = cms.PSet(),
			    genPartonMatch = cms.InputTag(""),
			    addTagInfos = cms.bool(False),
			    addPartonJetMatch = cms.bool(False),
			    embedGenPartonMatch = cms.bool(False),
			    efficiencies = cms.PSet(),
			    genJetMatch = cms.InputTag(""),
			    useLegacyJetMCFlavour = cms.bool(False),
			    userData = cms.PSet(
			    userCands = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userInts = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userFloats = cms.PSet(
			    src = cms.VInputTag("ak8PFJetsCHSPrunedLinks",
			    "ak8PFJetsCHSSoftDropLinks", "Njettiness:tau1", "Njettiness:tau2", "Njettiness:tau3", "Njettiness:tau4")
			    ),
			    userClasses = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userFunctionLabels = cms.vstring(),
			    userFunctions = cms.vstring()
			    ),
			    jetSource = cms.InputTag("ak8PFJetsCHS"),
			    addEfficiencies = cms.bool(False),
			    discriminatorSources = cms.VInputTag(),
			    trackAssociationSource = cms.InputTag(""),
			    tagInfoSources = cms.VInputTag(cms.InputTag("")),
			    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsAK8")),
			    embedPFCandidates = cms.bool(False),
			    addJetFlavourInfo = cms.bool(False),
			    addResolutions = cms.bool(False),
			    getJetMCFlavour = cms.bool(False),
			    addDiscriminators = cms.bool(False),
			    jetChargeSource = cms.InputTag(""),
			    JetFlavourInfoSource = cms.InputTag(""),
			    addJetCorrFactors = cms.bool(True),
			    jetIDMap = cms.InputTag(""),
			    addJetID = cms.bool(False)
)


selectedPatJetsAK8 = cms.EDFilter("PATJetSelector",
    src = cms.InputTag("patJetsAK8"),
    cut = cms.string("pt > 20")
)			    

redoPatJets = cms.Sequence(patJetCorrFactorsAK8 + patJetsAK8 + selectedPatJetsAK8)

packedPFCandidatesCHS = cms.EDFilter("CandViewSelector",
			  src = cms.InputTag("packedPFCandidates"),
			  cut = cms.string("fromPV > 0")
			 )	


AK4PFCHSJetParameters =  PFJetParameters.clone()
AK4PFCHSJetParameters.src = cms.InputTag('packedPFCandidatesCHS')
AK4PFCHSJetParameters.jetPtMin = cms.double(10.0)

ak4PFJetsOnPackedCandidatesCHS = cms.EDProducer("FastjetJetProducer",
					      AK4PFCHSJetParameters,
					      AnomalousCellParameters,
					      jetAlgorithm = cms.string("AntiKt"),
					      rParam       = cms.double(0.4)
					     )

patJetCorrFactorsAK4 = cms.EDProducer("JetCorrFactorsProducer",
				      src = cms.InputTag("ak4PFJetsOnPackedCandidatesCHS"),
				      emf = cms.bool(False),
				      extraJPTOffset = cms.string('L1FastJet'),
				      primaryVertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
				      levels = cms.vstring('L1FastJet',
				      'L2Relative',
				      'L3Absolute'),
				      useNPV = cms.bool(True),
				      rho = cms.InputTag("fixedGridRhoFastjetAll"),
				      useRho = cms.bool(True),
				      payload = cms.string('AK4PFchs'),
				      flavorType = cms.string('J')
				      )
patJetsAK4 = cms.EDProducer("PATJetProducer",
			    addJetCharge = cms.bool(False),
			    addGenJetMatch = cms.bool(False),
			    embedGenJetMatch = cms.bool(False),
			    addAssociatedTracks = cms.bool(False),
			    addBTagInfo = cms.bool(False),
			    partonJetSource = cms.InputTag("NOT_IMPLEMENTED"),
			    addGenPartonMatch = cms.bool(False),
			    JetPartonMapSource = cms.InputTag(""),
			    resolutions = cms.PSet(),
			    genPartonMatch = cms.InputTag(""),
			    addTagInfos = cms.bool(False),
			    addPartonJetMatch = cms.bool(False),
			    embedGenPartonMatch = cms.bool(False),
			    efficiencies = cms.PSet(),
			    genJetMatch = cms.InputTag(""),
			    useLegacyJetMCFlavour = cms.bool(False),
			    userData = cms.PSet(
			    userCands = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userInts = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userFloats = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userClasses = cms.PSet(
			    src = cms.VInputTag("")
			    ),
			    userFunctionLabels = cms.vstring(),
			    userFunctions = cms.vstring()
			    ),
			    jetSource = cms.InputTag("ak4PFJetsOnPackedCandidatesCHS"),
			    addEfficiencies = cms.bool(False),
			    discriminatorSources = cms.VInputTag(),
			    trackAssociationSource = cms.InputTag(""),
			    tagInfoSources = cms.VInputTag(cms.InputTag("")),
			    jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsAK4")),
			    embedPFCandidates = cms.bool(False),
			    addJetFlavourInfo = cms.bool(False),
			    addResolutions = cms.bool(False),
			    getJetMCFlavour = cms.bool(False),
			    addDiscriminators = cms.bool(False),
			    jetChargeSource = cms.InputTag(""),
			    JetFlavourInfoSource = cms.InputTag(""),
			    addJetCorrFactors = cms.bool(True),
			    jetIDMap = cms.InputTag(""),
			    addJetID = cms.bool(False)
)
			    
			    

redoPatAK4Jets = cms.Sequence(packedPFCandidatesCHS + ak4PFJetsOnPackedCandidatesCHS + patJetCorrFactorsAK4 + patJetsAK4)


### Cleaning
# We want to make sure that the jets are not the electrons or muons done previously

import PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi as jetCleaner_cfi

cleanJets = jetCleaner_cfi.cleanPatJets.clone()
cleanJets.src = "patJetsAK8"
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

AK4Jets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("patJetsAK4")
                        )



cleanAK4Jets = jetCleaner_cfi.cleanPatJets.clone()
cleanAK4Jets.src = "AK4Jets"
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

AK4JetsSequence = cms.Sequence(AK4Jets + cleanAK4Jets )










