# gap.py
# Script for output analysis - TP1 MC658
# Gets an output file inside a folder and gets sum of all gaps and execution times.
# Standard: <instance>,<best_primal>,<best_dual>,<...>,<...>,<...>,<total_time>,...
#
# Usage: python3 gap.py "folder"/"file"."extension"
# If needed: python3 gap.py 'folder'/*

from sys import argv
import re

if __name__ == "__main__" :
    argv.pop(0)
    
    for j in argv:
        sum_gap = 0
        sum_time = 0
        name = re.split('/|\.', j)[1]
        with open(j) as f:
            l = f.readlines()
    
        for i in range(len(l)):
            l[i] = l[i].split(',')
            sum_gap+= (int(l[i][1]) - int(l[i][2]))/int(l[i][1])
            sum_time+= float(l[i][6])
        
        
        print("Sum of all gaps in file {}: ".format(name), sum_gap)
        print("Average gap in file {}: ".format(name), sum_gap/len(l))
        print("Sum of total execution times in file {}: ".format(name), sum_time)
        print("Average time in file {}: ".format(name), sum_time/len(l))
        print()
        
