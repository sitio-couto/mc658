import sys
from itertools import permutations as perm
from math import inf

file = sys.argv[1]
fp = open(file)
tasks = []

n_tasks = int(fp.readline())
sched = [i for i in range(n_tasks)]

for i in range(n_tasks):
    aux = fp.readline()
    aux = aux.split()
    tasks.append((int(aux[0]),int(aux[1])))

min_sum = inf

for order in perm(sched):
    f1tr = 0
    f2tr = 0
    sumf2 = 0
    for i in order:
        (dm1,dm2) = tasks[i]
        f1tr += dm1
        if f1tr > f2tr : f2tr = f1tr + dm2
        else : f2tr += dm2
        sumf2 += f2tr
    if sumf2 < min_sum : min_sum = sumf2

print(min_sum)
