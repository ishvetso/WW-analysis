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

