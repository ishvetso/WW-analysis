import FWCore.ParameterSet.Config as cms
process = cms.Process( "BtagEff" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10000)
)

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.allowUnscheduled = cms.untracked.bool(False) 

process.load("aTGCsAnalysis.Common.goodMuons_cff")
process.load("aTGCsAnalysis.Common.goodElectrons_cff")
process.load("aTGCsAnalysis.Common.MET_cff")
process.load("aTGCsAnalysis.Common.goodJets_cff")
process.load("aTGCsAnalysis.Common.trigger_cff")
process.load("aTGCsAnalysis.Common.leptonicW_cff")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.GlobalTag.globaltag = '76X_mcRun2_asymptotic_RunIIFall15DR76_v1'

process.NoiseFilters = cms.EDFilter("NoiseFilter",
            noiseFilter = cms.InputTag("TriggerResults", "", "PAT"),
            filterNames = cms.vstring("Flag_HBHENoiseFilter", "Flag_HBHENoiseIsoFilter",  "Flag_CSCTightHaloFilter", "Flag_EcalDeadCellTriggerPrimitiveFilter", "Flag_goodVertices", "Flag_eeBadScFilter") 
            )


#
# Set up electron ID (VID framework)
#

from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format  to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate 
dataFormat = DataFormat.MiniAOD

switchOnVIDElectronIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

#
# Configure an example module for user analysis with electrons
#

process.ElectronVeto = cms.EDFilter("LeptonVeto",
				    looseLeptonSrc = cms.InputTag("looseElectrons"),
				    tightLeptonSrc = cms.InputTag("tightElectrons"),
                                    minNLoose = cms.int32(1),
                                    maxNLoose = cms.int32(1),
                                    minNTight = cms.int32(1),
                                    maxNTight = cms.int32(1),
				   )

process.MuonVeto = cms.EDFilter("LeptonVeto",
            looseLeptonSrc = cms.InputTag("looseMuons"),
            tightLeptonSrc = cms.InputTag("tightMuons"),
                                    minNLoose = cms.int32(0),
                                    maxNLoose = cms.int32(0),
                                    minNTight = cms.int32(0),
                                    maxNTight = cms.int32(0),
           )

process.leptonSequence = cms.Sequence(process.muSequence + process.eleSequence + process.ElectronVeto + process.MuonVeto  )

process.jetFilter = cms.EDFilter("CandViewCountFilter",
                                 src = cms.InputTag("goodJets"),
                                 minNumber = cms.uint32(1),
                                )

process.jetSequence = cms.Sequence(process.fatJetsSequence +
				                            process.jetFilter+
                          				   process.AK4JetsSequence)

process.BtagAnalyzer = cms.EDAnalyzer("BTaggingEffAnalyzer",
                  									  JetsTag = cms.InputTag("goodAK4Jets"),
                  									  DiscriminatorTag = cms.string("pfCombinedInclusiveSecondaryVertexV2BJetTags"),
                  									  DiscriminatorValue = cms.double(0.935),
                  									  PtNBins = cms.int32(5),
                                      ptBinning = cms.vdouble(30.,40., 50.,100., 200.,2000. ),
                                      EtaNBins = cms.int32(5),
                                      etaBinning = cms.vdouble(-2.4, -1.5, -0.8, 0.8,1.5, 2.4),
									                   )

# PATH
process.btagPath = cms.Path(process.NoiseFilters  + process.TriggerElectron + process.METele +  process.egmGsfElectronIDSequence +  process.leptonSequence +   process.jetSequence + process.BtagAnalyzer)

process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('/store/mc/RunIIFall15MiniAODv2/WJetsToLNu_HT-2500ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/10000/008262ED-4CB8-E511-BDF3-D067E5F90F2A.root'),
    
)


process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.TFileService = cms.Service("TFileService",
                                 fileName = cms.string("tree_ele.root")
                                  )
