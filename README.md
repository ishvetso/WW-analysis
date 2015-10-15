Instructions to setup the aTGC analysis code
========

```
This is the branch for 74X samples, October 2015
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src
cmsenv
# checkout WW-analysis code
git clone -b 74X git@github.com:ishvetso/aTGCsAnalysis.git
# Compile
scram b -j 10

