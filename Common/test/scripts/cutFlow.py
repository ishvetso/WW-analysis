from ROOT import TTree, TFile 
import shutil
ele_file = TFile("/afs/cern.ch/work/i/ishvetso/public/synchronization_WW_Karlsruhe/tree_ele.root")
ele_tree = ele_file.Get("treeDumper/BasicTree")

mu_file = TFile("/afs/cern.ch/work/i/ishvetso/public/synchronization_WW_Karlsruhe/tree_mu.root")
mu_tree = mu_file.Get("treeDumper/BasicTree")

out_file = open('/afs/cern.ch/work/i/ishvetso/RunII_preparation/Synchronization_March2015/CMSSW_7_3_0/src/WW-analysis/Common/test/scripts/cutFlowTable.tex', 'w')
out_file.write('\documentclass[12pt]{article} \n\usepackage[british,UKenglish,USenglish,english,american]{babel} \n\usepackage[utf8x]{inputenc} \n\usepackage{amssymb} \n\usepackage{multirow} \n\usepackage{graphicx} \n\\textwidth 165mm \n\\textheight 240mm \n\\topmargin -16mm \n\oddsidemargin 0pt \n\\evensidemargin 0pt \n\pagenumbering{gobble} \n\n\\begin{document}')
out_file.write('\\begin{table}[ht] \n')
out_file.write('\\begin{center} \n')
out_file.write('\\begin{tabular}{|c|c|c|} \n')
out_file.write('\hline \n')
out_file.write('Selection & muon channel & electron channel \\\\ \n')
out_file.write('\hline \n')

out_file.write('lepton selection  & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1")) + ' \\\\ \n \\hline\n')

out_file.write('MET selection  & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80")) + ' \\\\ \n \\hline\n')

out_file.write('leptonic W $p_{T} > 200$ GeV  & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200")) + ' \\\\ \n \\hline\n')

out_file.write('AK8 jet $p_{T} > 200$ GeV  & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 ")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200 ")) + ' \\\\ \n \\hline\n')

out_file.write('btag veto  & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 & nbtag == 0")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200  & nbtag == 0")) + ' \\\\ \n \\hline\n')

out_file.write('$\\Delta R(lepton, AK8) > \\frac{\\pi}{2} $& ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 & nbtag == 0 &  deltaR_LepWJet > pi/2")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200  & nbtag == 0 & deltaR_LepWJet > pi/2")) + ' \\\\ \n \\hline\n')

out_file.write('$\\Delta Phi(E_{T}^{miss}, AK8) > 2.0 $& ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 & nbtag == 0 &  deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2 ")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200  & nbtag == 0 & deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2 ")) + ' \\\\ \n \\hline\n')

out_file.write('$\\Delta Phi(AK8, W_{lep}) > 2.0 $& ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 & nbtag == 0 &  deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2. & abs(deltaPhi_WJetWlep) > 2. ")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200  & nbtag == 0 & deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2. & abs(deltaPhi_WJetWlep) > 2. ")) + ' \\\\ \n \\hline\n')

out_file.write('$\\tau_{21} < 0.5 $ & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 & nbtag == 0 &  deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2  & abs(deltaPhi_WJetWlep) > 2.  & tau21 < 0.5 ")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200  & nbtag == 0 & deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2  & abs(deltaPhi_WJetWlep) > 2. & tau21 < 0.5 ")) + ' \\\\ \n \\hline\n')

out_file.write('$ 40 < m_{jet}^{pruned} < 130 $ & ' + str(mu_tree.GetEntries("nLooseMu == 1 & nLep == 1 & pfMET > 40 & W_pt > 200 & jet_pt > 200 & nbtag == 0 &  deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2  & abs(deltaPhi_WJetWlep) > 2. & tau21 < 0.5 & jet_mass_pruned > 40 & jet_mass_pruned < 130")) + ' & '  + str(ele_tree.GetEntries("nLooseEle == 1 & nLep == 1 & pfMET > 80 & W_pt > 200 & jet_pt > 200  & nbtag == 0 & deltaR_LepWJet > pi/2 & abs(deltaPhi_WJetMet) > 2 & abs(deltaPhi_WJetWlep) > 2.  & tau21 < 0.5 & jet_mass_pruned > 40 & jet_mass_pruned < 130")) + ' \\\\ \n \\hline\n')
out_file.write('\\end{tabular} \n')
out_file.write('\\end{center} \n')
out_file.write('\\end{table} \n\n')
out_file.write('\\end{document} \n\n')
out_file.write(' \n')