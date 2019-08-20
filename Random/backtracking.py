# Victor Ferreira Ferrari
# 17/03/2018
# Implementation of backtracking algorithms for NP-hard problems.

import numpy as np

# Algorithm for solving the "sum of subsets" problem implementation
# Given a set st with n positive integers and an integer W,
# Finds if there is a subset such that the sum of all values is W.
# Assumes sum(st) >= W
# Prints all subsets that answer the problem.
def sos(st, W):
	n  = len(st)
	x  = np.zeros(n, dtype=bool)
	st = np.array(st)
	
	# Sorts the set.
	st.sort()
	
	# Recursive part of the function
	def sos_bt(s,k,r):
		x[k] = True		
		
		# Found a subset
		if s + st[k] == W:
			print(st[x])
			
		# Set is done.
		if k+1 == n:
			x[k] = False
			return None
		
		# Includes the k-item in the subset, and if the sum with the next one doesn't surpass W, keeps going.
		# Possible to do this since the set is sorted.			
		if s + st[k] + st[k+1] <= W:
			sos_bt(s+st[k], k+1, r-st[k])
		
		# Removes the k-item from the subset, and if it's still possible to achieve W, keeps going.
		x[k] = False
		if s + r - st[k] >= W and s + st[k+1] <= W:
			sos_bt(s, k+1, r-st[k])
	
	sos_bt(0, 0, sum(st))

# Given a graph represented by adjacency matrix, finds all coloring variations with p or less colors.
def graph_coloring(graph, p):
	return None
