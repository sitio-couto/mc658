import sys
import re
from itertools import chain

fin = open(sys.argv[1],"r")
fout = open(sys.argv[2],"w")

lines = fin.readlines()
lines = [re.split(",|{|}", lines[i])[7:] for i in range(len(lines))]
lines = [lines[i][1:-1] for i in range(len(lines))]
output = [[0]*len(lines[i]) for i in range(len(lines))]

c = 0
for l in lines:
    for i in l:
        output[c][int(i)-1] = l.index(i)+1
    c+=1

for i in range(len(output)):
    fout.write(str(output[i])+"\n")
