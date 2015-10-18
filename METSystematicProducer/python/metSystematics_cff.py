import FWCore.ParameterSet.Config as cms

#Note that systematics on leptons is not added, 16.10.2015.

def CreateWLepWithMETSystematicsSequence(process, channel):
	process.metSequenceSystematics = cms.Sequence()
	ListOfSystematics = ["JetResUp", "JetResDown", "JetEnUp", "JetEnDown", "MuonEnUp", "MuonEnDown", "ElectronEnUp", "ElectronEnDown", "UnclusteredEnUp", "UnclusteredEnDown"]
	_MET = cms.EDProducer("METSystematicProducer", 
                                     metSrc = cms.InputTag("slimmedMETs"),
                                     uncertaintyType = cms.string("") )
	_Wtomunu = cms.EDProducer("WLeptonicProducer",
                         leptons = cms.InputTag("bestMuon"),
                         MET = cms.InputTag("slimmedMETs"),
                         cut = cms.string("")
                         )
	_Wtoenu = cms.EDProducer("WLeptonicProducer",
                        leptons = cms.InputTag("bestElectron"),
                        MET = cms.InputTag("slimmedMETs"),
                        cut = cms.string("")
                        )
	for iSyst in ListOfSystematics:
		setattr(process, 'MET' + iSyst, _MET.clone(uncertaintyType = iSyst))
		if channel == "mu" : 
			setattr(process, 'Wtomunu' + iSyst, _Wtomunu.clone(MET = cms.InputTag("MET" + iSyst)))
			process.metSequenceSystematics *= getattr(process, 'MET' + iSyst) * getattr(process, 'Wtomunu' + iSyst) 
		elif  channel == "el":
			setattr(process, 'Wtoelnu' + iSyst, _Wtoenu.clone(MET = cms.InputTag("MET" + iSyst)))
			process.metSequenceSystematics *= getattr(process, 'MET' + iSyst) * getattr(process, 'Wtoelnu' + iSyst)
		else :
			raise ValueError('Only muon and electron channel are supported, mu or el.')

	return process.metSequenceSystematics
