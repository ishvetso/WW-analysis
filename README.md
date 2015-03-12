Instructions to setup the WW-analysis code
========

```
This is the branch for synchronization with Zurich, Milano and Peking groups. March 2015
cmsrel CMSSW_7_2_2_patch1
cd CMSSW_7_2_2_patch1/src
cmsenv
# checkout the package for electrons, for HEEP electron ID
git cms-merge-topic Sam-Harper:HEEPID73X
# checkout WW-analysis code
git clone git@github.com:ishvetso/WW-analysis.git
# Compile
scram b -j 10

