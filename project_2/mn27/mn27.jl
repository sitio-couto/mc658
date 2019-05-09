# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# PROBLEM [mn27]
# Minimize the number o k colors necessary for coloring a graph

# VARIABLES DESCRIPTION
# n = number of vertices
# m = number of edges
# E = Array o tuples (i,j) representing the edges

# Importing packages
using JuMP, Gurobi, Printf

# Instance path
file_name = "Instances/mn27."*ARGS[1]*".instance"
#file_name = ARGS[1]

# Time Limit
if length(ARGS) > 1
	TL = parse(Int64, ARGS[2])
else
	TL = 100000
end

# INPUT: data processing and representation block
n,m,E = open(file_name, "r") do file
	data  = readlines(file)

	# n,m in first line
	(n,m) = map(x->parse(Int64,x), split(data[1]))

	# Creating edge array
	E = Array{Tuple{Int64, Int64}}(undef,m)
	
	# Reading edges
	for (i, edge) in enumerate(data[2:m+1])
		E[i] = Tuple(map(x->parse(Int64,x), split(edge)))
	end

	# Return structured values
	(n,m,E)
	
end # End of open block

# MODEL: building and execution block
let
	# COMBINATIONS FOR THE MODEL
	# V X V combination for color j for each vertex i
	colorVertex = Array{Tuple{Int64, Int64}}(undef, n*n)
	k=1
	for i = 1:n
		for j = 1:n
			colorVertex[k]=(i,j)
			k+=1
		end
	end

	#--------------------------------------------------------------------

	# MODEL BUILDING
	mn27 = Model(solver=GurobiSolver(TimeLimit=TL))

	# Variables
	@variable(mn27, y[1:n], Bin)				# If color i is used
	@variable(mn27, x[colorVertex], Bin)# If vertex i is coloured by color m

	# Objective Function
	@objective(mn27, Min, sum(y))

	# CONSTRAINTS
	# Each vertex should be colored with one and only one color
	for i = 1:n
		@constraint(mn27, sum(x[(i,m)] for m = 1:n) == 1)
	end

	# The head and tail of an edge can't have the same color
	for (i,j) in E
		for m = 1:n
			@constraint(mn27, x[(i,m)] + x[(j,m)] <= 1)
		end
	end

	# A color is used if a vertex is colored with it
	# n*y[m] >= sum(x[i,m])
	for m = 1:n
		@constraint(mn27, n*y[m] - sum(x[(i,m)] for i=1:n) >= 0)
	end

	# Solving model
	status = solve(mn27)

	#--------------------------------------------------------------------

	# REPORT
	println("========================================================================")
	if status == :Optimal
	  println("Solução ótima encontrada.")
	elseif status == :Unbounded
	  println("Problema é ilimitado.")
	elseif status == :Infeasible
	  println("Problema é inviável.")
	elseif status == :UserLimit
	  println("Parado por limite de tempo ou iterações.")
	elseif status == :Error
	  println("Erro do resolvedor.")
	else
	  println("Não resolvido.")
  end

	println("Número de nós explorados: ", getnodecount(mn27::Model))
	D = getobjbound(mn27::Model)
	P = getobjectivevalue(mn27::Model)
	@printf("Melhor limitante dual: %.2f\n", D)
	@printf("Melhor limitante primal: %.2f\n", P)
	Gap = (abs( D - P )/P)*100
	@printf("Gap de otimalidade: %.2f\n", Gap)
	@printf("Tempo de execução: %.2f\n", getsolvetime(mn27::Model))
	            
end
