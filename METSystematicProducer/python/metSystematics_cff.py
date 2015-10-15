import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import cloneProcessingSnippet

def CreateWLepWithMETSystematicsSequence(process):
	process.metSequenceSystematics = cms.Sequence()
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
		setattr(process, 'MET' + ListOfSystematics[iSyst], clonedMET.clone(uncertaintyType = ListOfSystematics[iSyst]))
		setattr(process, 'Wtomunu' + ListOfSystematics[iSyst], WtomunuCloned.clone(MET = 'MET' + ListOfSystematics[iSyst]))
		setattr(process, 'Wtoenu' + ListOfSystematics[iSyst], WtoenuCloned.clone(MET = 'MET' + ListOfSystematics[iSyst]))
		SystSequence = cms.Sequence(getattr(process, 'MET' + ListOfSystematics[iSyst]), getattr(process, 'Wtomunu' + ListOfSystematics[iSyst]), getattr(process, 'Wtoenu' + ListOfSystematics[iSyst]))
		process.metSequenceSystematics =+ cloneProcessingSnippet(process, SystSequence, ListOfSystematics[iSyst])

	return process.metSequenceSystematics
