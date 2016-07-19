Instructions to setup the aTGC analysis code
========

```
This is the branch for 80X samples, summer 2016
cmsrel CMSSW_8_0_11
cd CMSSW_8_0_11/src
cmsenv
# checkout aTGC analysis code
git cms-merge-topic -u cms-met:CMSSW_8_0_X-METFilterUpdate
git clone -b 80X git@github.com:ishvetso/aTGCsAnalysis.git
# Compile
scram b -j 10
Plotting: code is located in aTGCsAnalysis/Common/test/Plotting/.
To compile do:
make
Allowed options:
  --help                produce help message
  --channel arg         channel to be used: ele or mu
  --CR arg              control region to make plots with
  --output arg          tag for the output directory
  --input arg           name of input directory
  --withMC              use Monte Carlo or not
  --withData            use data or not
  --withSignal          draw signal or not
  --withSystematics     calculate systematics or not. If not statistical 
                        uncertainties are calculated and drawn.
  --wantToWriteHists    to write histograms to the local file
An example below makes plots in the ttbar control region in the electron channel with data, Monte-Carlo, signal and no systematics :
./draw --CR ttbar --channel ele --output ttbar_CR --input /afs/cern.ch/work/i/ishvetso/aTGCRun2/samples_76X_24April2016/ --withSignal --withMC --withData
