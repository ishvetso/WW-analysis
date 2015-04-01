from ROOT import TTree, TFile 
import shutil
ele_file = TFile("/afs/cern.ch/work/i/ishvetso/RunII_preparation/Synchronization_March2015/trees/tree_ele.root")
ele_tree = ele_file.Get("treeDumper/BasicTree")

mu_file = TFile("/afs/cern.ch/work/i/ishvetso/RunII_preparation/Synchronization_March2015/trees/tree_mu.root")
mu_tree = ele_file.Get("treeDumper/BasicTree")

print "Electron" 
print ele_tree.GetEntries()
print ele_tree.GetEntries("nLooseEle == 1 & nLep == 1")
print ele_tree.GetEntries("pfMET > 40")


out_file = open('/afs/cern.ch/work/i/ishvetso/RunII_preparation/Synchronization_March2015/cutFlowTable.tex', 'w')
out_file.write('\documentclass[12pt]{article} \n\usepackage[british,UKenglish,USenglish,english,american]{babel} \n\usepackage[utf8x]{inputenc} \n\usepackage{amssymb} \n\usepackage{multirow} \n\usepackage{graphicx} \n\\textwidth 165mm \n\\textheight 240mm \n\\topmargin -16mm \n\oddsidemargin 0pt \n\\evensidemargin 0pt \n\pagenumbering{gobble} \n\n\\begin{document}')
out_file.write('\\begin{table}[ht] \n')
out_file.write('\\begin{center} \n')
out_file.write('\\begin{tabular}{|c|c|c|} \n')
out_file.write('\hline \n')
out_file.write('Selection & muon channel & electron channel \\\\ \n')
out_file.write('\hline \n')
out_file.write('\\end{tabular} \n')
out_file.write('\\end{center} \n')
out_file.write('\\end{table} \n\n')
out_file.write('\\end{document} \n\n')
out_file.write(' \n')