Instructions to setup the WW-analysis code
========

```
This is the branch for synchronization with Zurich, Milano and Peking groups. March 2015
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src
cmsenv
# checkout WW-analysis code
git clone -b 74X git@github.com:ishvetso/WW-analysis.git
# Compile
scram b -j 10

