# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# K-Coloração de grafos para k mínimo, PLI

# Importando pacotes
using JuMP, Gurobi, Printf
file_name = "Instancias/mn27."*ARGS[1]*".instance"

<<<<<<< HEAD
=======
file_name = "Instancias/mn27."*ARGS[1]*".instance"

>>>>>>> f7f82834b068b42fbb6b4519f843e128d866f3a5
# Time Limit
if length(ARGS) > 1
	TL = parse(Int64, ARGS[2])
else
	TL = 100000
end

# n = number of vertices
# m = number of edges
n,m,edges = open(file_name, "r") do file
	data  = readlines(file)

	# n,m in first line
	(n,m) = map(x->parse(Int64,x), split(data[1]))

	# Creating edge array
	edges = Array{Tuple{Int64, Int64}}(undef,m)
	
	# Reading edges
	for (i, edge) in enumerate(data[2:m+1])
		edges[i] = Tuple(map(x->parse(Int64,x), split(edge)))
	end

	# Return structured values
	(n,m,edges)
	
end # End of open block

# Start model build and execution
let
	# V X V combination for color j for each vertex i
	colorVertex = Array{Tuple{Int64, Int64}}(undef, n*n)
	k=1
	for i = 1:n
		for j = 1:n
			colorVertex[k]=(i,j)
			k+=1
		end
	end

	# Model
	mn27 = Model(solver=GurobiSolver(TimeLimit=TL))

	# Variables
	@variable(mn27, y[1:n], Bin)				# If color i is used
	@variable(mn27, x[i in colorVertex], Bin)	# If vertex i is coloured by color m

	# Objective Function
	@objective(mn27, Min, sum(y))

	# CONSTRAINTS
	# Each vertex should be colored with one and only one color
	for i = 1:n
		@constraint(mn27, sum(x[(i,m)] for m = 1:n) == 1)
	end

	# The head and tail of an edge can't have the same color
	for (i,j) in edges
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

	# --------------------------------------------------------------------

	# Relatório
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
