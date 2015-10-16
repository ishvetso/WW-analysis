import FWCore.ParameterSet.Config as cms

def CreateWLepWithMETSystematicsSequence(process, channel):
	process.metSequenceSystematics = cms.Sequence()
	ListOfSystematics = ["JetResUp", "JetResDown", "JetEnUp", "JetEnDown", "MuonEnUp", "MuonEnDown", "ElectronEnUp", "ElectronEnDown", "UnclusteredEnUp", "UnclusteredEnDown"]
	_MET = cms.EDProducer("METSystematicProducer", 
                                     metSrc = cms.InputTag("slimmedMETs"),
                                     uncertaintyType = cms.string("") )
	_Wtomunu = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("bestMuon"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("pt > 200")
                         )
	_Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("pt > 200")
                        )
	for iSyst in ListOfSystematics:
		setattr(process, 'MET' + iSyst, _MET.clone(uncertaintyType = iSyst))
		if channel == "mu" : 
			setattr(process, 'Wtomunu' + iSyst, _Wtomunu.clone(MET = cms.InputTag("MET" + iSyst)))
			process.metSequenceSystematics *= getattr(process, 'MET' + iSyst) * getattr(process, 'Wtomunu' + iSyst) 
		elif  channel == "el":
			setattr(process, 'Wtoenu' + iSyst, _Wtoenu.clone(MET = cms.InputTag("MET" + iSyst)))
			process.metSequenceSystematics *= getattr(process, 'MET' + iSyst) * getattr(process, 'Wtoenu' + iSyst)
		else :
			raise ValueError('Only muon and electron channel are supported, mu or el.')

	return process.metSequenceSystematics
