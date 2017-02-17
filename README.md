Instructions to setup the aTGC analysis code
========

```
This is the branch for 76X samples, February 2016
cmsrel CMSSW_7_6_4
cd CMSSW_7_6_4/src
cmsenv
git cms-merge-topic Sam-Harper:HEEPV61 # to fix the drop of electron ID efficiency ~100 GeV
# checkout aTGC analysis code
git clone -b 76X git@github.com:ishvetso/aTGCsAnalysis.git
# Compile
scram b -j 10

Samples (the last version):
/afs/cern.ch/work/i/ishvetso/public/samples_76X_22July2016

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

Note that if you use "withSignal" option systematics for the signal will be computed automatically.

To study the effect of systematic uncertainties on the signal shape parameters make use the script located in aTGCsAnalysis/Common/test/Plotting/ :
 python systematics-signal-shapes.py --cat WW --ch ele -l
 
