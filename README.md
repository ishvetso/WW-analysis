Instructions to setup the WW-analysis code
========

```
cmsrel CMSSW_7_2_2_patch1
cd CMSSW_7_2_2_patch1/src
cmsenv
git clone git@github.com:ishvetso/WW-analysis.git
# Before compiling, you need to setup the N-subjettiness
cd $CMSSW_BASE/src
cp $CMSSW_BASE/src/WW-analysis/Jets/tools/* $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/
# Tell SCRAM that those tools are going to be setup
scram setup fastjet-contrib
scram setup fastjet-contrib-archive
# Then compile:
scram b -j 10
