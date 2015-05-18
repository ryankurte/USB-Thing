#!/usr/bin/python
 
import argparse
import subprocess
 
#Parse input argument
parser = argparse.ArgumentParser(description='Generate `git describe` based version header file')
parser.add_argument('output', type=str, help='output file name')
 
args = parser.parse_args()
output = args.output
 
#Load git description
p = subprocess.Popen('git describe --dirty', stdout=subprocess.PIPE, shell=True)
version = p.communicate()[0]
versionString = version.replace('\n', '').replace('\r', '');
 
#Generate new file string
baseString = '#ifndef GIT_VERSION_H\r\n#define GIT_VERSION_H\r\n#define SOFTWARE_VERSION \"version_here\"\r\n#endif\r\n'
newString = baseString.replace('version_here', versionString)
 
#Load existing file string
try:
	f = open(output, 'r')
	f.seek(0)
	fileString = f.read();
	f.close();
except IOError, e:
	fileString = "";
 
#Check for differences and update if required
if(fileString != newString):
	f = open(output, 'w+')
	f.seek(0)
	f.write(newString)
	f.truncate()
	f.close()