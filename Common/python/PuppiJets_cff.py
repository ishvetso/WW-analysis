import FWCore.ParameterSet.Config as cms
from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector
from Dummy.Puppi.Puppi_cff import puppi

from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
from RecoJets.JetProducers.PFJetParameters_cfi import *
ak8PFJetsPuppi = ak4PFJets.clone( rParam = 0.8 )
ak8PFJetsPuppi.src =  cms.InputTag('puppi','Puppi')


# Need to convert ak8PFJetsPuppi to pat??

PuppiJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("ak8PFJetsPuppi")
                        )
PuppiSequence = cms.Sequence(puppi + ak8PFJetsPuppi)