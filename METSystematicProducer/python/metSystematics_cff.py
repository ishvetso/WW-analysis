import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

def CreateWLepWithMETSystematicsSequence(process):
	process.metSequenceSystematics = cms.Sequence()
	ListOfSystematics = ["JetResUp", "JetResDown", "JetEnUp", "JetEnDown", "MuonEnUp", "MuonEnDown", "ElectronEnUp", "ElectronEnDown", "UnclusteredEnUp", "UnclusteredEnDown"]
	_clonedMET = cms.EDProducer("METSystematicProducer", 
                                     metSrc = cms.InputTag("slimmedMETs"),
                                     uncertaintyType = cms.string("JetResUp") )
	_WtomunuCloned = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("bestMuon"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 200")
                         )


	_WtoenuCloned = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("pt > 200")
                        )
	process.shiftedMETs = cms.Sequence()
	for iSyst in ListOfSystematics:
		setattr(process, 'MET' + iSyst, _clonedMET.clone(uncertaintyType = iSyst))
		setattr(process, 'Wtomunu' + iSyst, _WtomunuCloned.clone(MET = 'MET' + iSyst))
		setattr(process, 'Wtoenu' + iSyst, _WtoenuCloned.clone(MET = 'MET' + iSyst))
		process.metSequenceSystematics *= getattr(process, 'MET' + iSyst) * getattr(process, 'Wtomunu' + iSyst) * getattr(process, 'Wtoenu' + iSyst)

	return process.metSequenceSystematics
