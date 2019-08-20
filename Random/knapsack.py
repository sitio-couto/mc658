# Victor Ferreira Ferrari
# 12/03/2018 - 17/03/2018
# Implementation of algoriths for solving the knapsack problem.
# Given a group of items (their weights and values) and a knapsack with fixed size.
# Returns the max value of the sum of all items.

from math import inf

# Simple dynamic programming algorithm implementation.
# Weights and size have to be integers.
# Complexity: O(n*size) - Pseudo-polinomial
def knapsack_simple(weights, values, size):
	n = len(weights)
	
	# Invalid lists
	if n != len(values):
		print("How many items? Lists have different lengths.")
		return None
	
	# Non-integer size or weights
	if not isinstance(sum(weights,size), int):
		print("Weights and size have to be integers!")
		return None
	
	# Empty
	if (n == 0 or size <= 0):
		return 0
	
	table = [[0] * (size+1)]
	
	# Basic case
	for i in range(n):
		table.append([0])
		
	# Filling the table
	for i in range(1,n+1):
		for j in range(1,size+1):
			prev = table[i-1][j]								# Previous knapsack with the same weight.
			addItem = table[i-1][j-weights[i-1]]				# Knapsack without current item.
			
			# Deciding if the current item belongs in the knapsack.
			if weights[i-1] <= j:
				nxt = max(prev, addItem + values[i-1])	
			else:
				nxt = prev
			table[i].append(nxt)
			
	#print(table)
	
	# Return max value
	return table[n][size]

# Alternate dynamic programming algorithm implementation.
# Only the values of the items have to be integers.
# Complexity: O(n²C) (C = highest value) - Pseudo-polinomial
def knapsack_values(weights, values, size):
	n = len(weights)
	C = sum(values)
	
	# Invalid lists
	if n != len(values):
		print("How many items? Lists have different lengths.")
		return None
	
	# Non-integer values
	if not isinstance(sum(values),int):
		print("Values have to be integers!")
		return None
	
	# Empty
	if (n == 0 or size <= 0):
		return 0
		
	table = [[0]]			# A[0][0] = 0
	table[0] += [inf] * C 	# A[0][i] = inf, i>0
	
	# Basic case
	for i in range(n):
		table.append([0])
		
	# Filling the table
	for i in range(1,n+1):
		for j in range(1,C+1):
			prev = table[i-1][j]							# Previous knapsack with the same value.
			addItem = table[i-1][j-values[i-1]]				# Knapsack without current item.
			
			# Deciding if the current item belongs in the knapsack.
			if values[i-1] <= j:
				nxt = min(prev, addItem + weights[i-1])	
			else:
				nxt = prev
			table[i].append(nxt)
			
	#print(table)
	
	# Finds max value
	i = C
	while table[n][i] > size:
		i-=1
	
	return i
