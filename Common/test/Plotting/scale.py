import ROOT

ndv = 166.
edv = 12.8841

ndi = 211.
edi = 14.5258

ntv = 96.2485
etv = 0.756708

nti = 152.142
eti = 0.942926

nwv = 74.4318
ewv = 1.33099

nwi = 76.368 
ewi = 1.34363

print "scale factor " + str( ((ndv - nwv)/(ndi - nwi)) / (ntv/nti))

hdv = ROOT.TH1F("hdv","hdv",1,0,1)
hdv.Sumw2(True)
hdv.SetBinContent(1,ndv)
hdv.SetBinError(1,edv)

hwv = ROOT.TH1F("hwv","hwv",1,0,1)
hwv.Sumw2(True)
hwv.SetBinContent(1,nwv)
hwv.SetBinError(1,ewv)

hdv.Add(hwv,-1)

hdi = ROOT.TH1F("hdi","hdi",1,0,1)
hdi.Sumw2(True)
hdi.SetBinContent(1,ndi)
hdi.SetBinError(1,edi)

hwi = ROOT.TH1F("hwi","hwi",1,0,1)
hwi.Sumw2(True)
hwi.SetBinContent(1,nwi)
hwi.SetBinError(1,ewi)

hdi.Add(hwi,-1)

effd = ROOT.TEfficiency(hdv,hdi)

htv = ROOT.TH1F("htv","htv",1,0,1)
htv.Sumw2(True)
htv.SetBinContent(1,ntv)
htv.SetBinError(1,etv)

hti = ROOT.TH1F("hti","hti",1,0,1)
hti.Sumw2(True)
hti.SetBinContent(1,nti)
hti.SetBinError(1,eti)

efft = ROOT.TEfficiency(htv,hti)

print "data " + str(effd.GetEfficiency(1)) +" - "+ str(effd.GetEfficiencyErrorLow(1)) +" + "+ str(effd.GetEfficiencyErrorUp(1))
print "data " + str(efft.GetEfficiency(1)) +" - "+ str(efft.GetEfficiencyErrorLow(1)) +" + "+ str(efft.GetEfficiencyErrorUp(1))

print " final ratio = " + str(effd.GetEfficiency(1)/ efft.GetEfficiency(1) )
print " final errot = " + str(effd.GetEfficiency(1)/ efft.GetEfficiency(1) * ROOT.TMath.Sqrt( 
        effd.GetEfficiencyErrorLow(1)/effd.GetEfficiency(1)*effd.GetEfficiencyErrorLow(1)/effd.GetEfficiency(1) +
        efft.GetEfficiencyErrorLow(1)/efft.GetEfficiency(1)*efft.GetEfficiencyErrorLow(1)/efft.GetEfficiency(1)         
        ))
