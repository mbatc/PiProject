MAKEFILE = open("make_template","r");

from os import listdir
files = listdir("../source")

source = '_SOURCES='
header = '_HEADERS='


for i in range(0,len(files)):
	if files[i].find(".cpp",len(files[i])-5) > 0:
		source += ' ' + files[i]
	if files[i].find(".h",len(files[i])-4) > 0:
		header += ' ' + files[i]

source += '\n'
header += '\n'
final_make = source + header

print(final_make)
print(MAKEFILE.read())
