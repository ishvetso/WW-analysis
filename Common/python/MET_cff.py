import FWCore.ParameterSet.Config as cms

from RecoJets.JetProducers.ak4PFJets_cfi import *
from RecoMET.METProducers.PFMET_cfi import pfMet

pfMet = pfMet.clone(src = "packedPFCandidates")
pfMet.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment

MyPFJetParameters =  PFJetParameters.clone()
MyPFJetParameters.src = cms.InputTag('packedPFCandidates')

ak4PFJetsOnPackedCandidates = cms.EDProducer("FastjetJetProducer",
					      MyPFJetParameters,
					      AnomalousCellParameters,
					      jetAlgorithm = cms.string("AntiKt"),
					      rParam       = cms.double(0.4)
					     )

ak4PFL1FastjetCorrector = cms.EDProducer(
    'L1FastjetCorrectorProducer',
    level       = cms.string('L1FastJet'),
    algorithm   = cms.string('AK4PF'),
    srcRho      = cms.InputTag( 'fixedGridRhoFastjetAll' )
    )

ak4PFL2RelativeCorrector = cms.EDProducer(
    'LXXXCorrectorProducer',
    level     = cms.string('L2Relative'),
    algorithm = cms.string('AK4PF')
    )

ak4PFL3AbsoluteCorrector = cms.EDProducer(
    'LXXXCorrectorProducer',
    level     = cms.string('L3Absolute'),
    algorithm = cms.string('AK4PF')
    )

ak4PFL1FastL2L3Corrector = cms.EDProducer(
    'ChainedJetCorrectorProducer',
    correctors = cms.VInputTag('ak4PFL1FastjetCorrector','ak4PFL2RelativeCorrector','ak4PFL3AbsoluteCorrector')
    )

corrPfMetType1 = cms.EDProducer(
    "PFJetMETcorrInputProducer",
    src = cms.InputTag('ak4PFJetsOnPackedCandidates'),
    offsetCorrLabel = cms.InputTag("ak4PFL1FastjetCorrector"),
    jetCorrLabel = cms.InputTag("ak4PFL1FastL2L3Corrector"), # NOTE: use "ak4PFL1FastL2L3Corrector" for MC / "ak4PFL1FastL2L3ResidualCorrector" for Data
    jetCorrEtaMax = cms.double(9.9),
    type1JetPtThreshold = cms.double(10.0),
    skipEM = cms.bool(True),
    skipEMfractionThreshold = cms.double(0.90),
    skipMuons = cms.bool(True),
    skipMuonSelection = cms.string("isGlobalMuon | isStandAloneMuon")
)


pfMetT1 = cms.EDProducer(
    "AddCorrectionsToPFMET",
    src = cms.InputTag('pfMet'),
    srcCorrections = cms.VInputTag(
        cms.InputTag('corrPfMetType1', 'type1')
    ),
)   

metSequence = cms.Sequence(pfMet + ak4PFJetsOnPackedCandidates + ak4PFL1FastjetCorrector + ak4PFL2RelativeCorrector + ak4PFL3AbsoluteCorrector + ak4PFL1FastL2L3Corrector + corrPfMetType1 + pfMetT1)