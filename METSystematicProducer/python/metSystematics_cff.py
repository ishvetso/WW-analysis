import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

def CreateWLepWithMETSystematicsSequence(process):
	process.metSequenceSystematics = cms.Sequence()
	process.EmptySequence = cms.Sequence()
	ListOfSystematics = ["JetResUp", "JetResDown", "JetEnUp", "JetEnDown", "MuonEnUp", "MuonEnDown", "ElectronEnUp", "ElectronEnDown", "UnclusteredEnUp", "UnclusteredEnDown"]
	clonedMET = cms.EDProducer("METSystematicProducer", 
                                     metSrc = cms.InputTag("slimmedMETs"),
                                     uncertaintyType = cms.string("JetResUp") )
	WtomunuCloned = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("bestMuon"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 200")
                         )


	WtoenuCloned = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("pt > 200")
                        )
	process.shiftedMETs = cms.Sequence()
	for iSyst in ListOfSystematics:
		setattr(process, 'MET' + iSyst, clonedMET.clone(uncertaintyType = iSyst))
		setattr(process, 'Wtomunu' + iSyst, WtomunuCloned.clone(MET = 'MET' + iSyst))
		setattr(process, 'Wtoenu' + iSyst, WtoenuCloned.clone(MET = 'MET' + iSyst))
		#setattr(process, 'SystSequence' + iSyst, getattr(process, 'MET' + iSyst) + getattr(process, 'Wtomunu' + iSyst) + getattr(process, 'Wtoenu' + iSyst))
		#process.SystSequence = cms.Sequence(getattr(process, 'MET' + iSyst) + getattr(process, 'Wtomunu' + iSyst) + getattr(process, 'Wtoenu' + iSyst))
		process.metSequenceSystematics *= getattr(process, 'MET' + iSyst) * getattr(process, 'Wtomunu' + iSyst) * getattr(process, 'Wtoenu' + iSyst)

	return process.metSequenceSystematics
