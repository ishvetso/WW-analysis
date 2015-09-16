import os
import sys
import fileinput
import optparse
import sys

def createFileForJob(textToReplace, fileName ):
	textToSearch = "$FEATURE$" 
	fileTemplate = 'templates/' + fileName
	tempFile = open( fileName, 'w+' )
	infile = open(fileTemplate)
	for line in infile:
		tempFile.write( line.replace( textToSearch, textToReplace))
	tempFile.close()
	return;


parser =  optparse.OptionParser()
parser.add_option('-p', '--pattern',
                  dest="pattern",
                  default='my_feature')
(options, arg) = parser.parse_args()

#createFileForJob(options.pattern, 'crabConfig_STop_s-channel_ele.py')

#os.system("crab submit -c " + 'crabConfig_STop_s-channel_ele.py')

MyListOfJobs = ["crabConfig_STop_s-channel_mu.py", "crabConfig_STop_t-channel_antitop_ele.py", "crabConfig_STop_t-channel_antitop_mu.py", "crabConfig_STop_t-channel_top_ele.py",
 "crabConfig_STop_t-channel_top_mu.py", "crabConfig_STop_tW-channel_antitop_ele.py", "crabConfig_STop_tW-channel_antitop_mu.py", "crabConfig_STop_tW-channel_top_ele.py",
 "crabConfig_STop_tW-channel_top_mu.py"]

def submitJobs(ListOfJobs):
	for iList in ListOfJobs:
		print iList
		createFileForJob(options.pattern, iList)
		os.system("crab submit -c " + iList)

submitJobs(MyListOfJobs)
