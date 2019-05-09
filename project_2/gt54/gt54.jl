# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# PROBLEM [gt27]
# Find minimum path between two vertices given certain restrictions

# VARIABLES DESCRIPTION
# n = Amount of nodes in instance
# m = Amount of edges in instance
# s = Starting vertex
# t = Ending vertex
# c = Cardinality of C set
# C = C set (vertex pairs restrictions) represented by an array of tuples
# E = Array of tuples representing the existing edges in the graph 
# W = Adjacency matrix with weights representing the instance graph 
# NOTE: The adjacency matrix W was used alongside the edges list E in this 
# particular exercise due to the last two restrictions (entry/exit degree).
# Using only the list E, it took several minutes for the model to be built,
# so the matrix was used to reduce the amount of operation done per vertex
# when creating the last two restrictions.

# Importing packages
using JuMP, Gurobi, Printf

# Instance path
file_name = "Instances/gt54."*ARGS[1]*".instance"
#file_name = ARGS[1]

# Time Limit
if length(ARGS) > 1
    TL = parse(Int64, ARGS[2])
else
    TL = 100000
end

# INPUT: data processing and representation block
n,m,c,s,t,C,E,W = open(file_name) do file
    data = readlines(file)
    (n,c,m) = map(x->parse(Int64,x), split(data[1])) # Parse |V|, |C| and |E|
    (s,t) = map(x->parse(Int64,x), split(data[2]))   # Parse origin and destination
    C = Array{Tuple{Int64, Int64}}(undef,c) # Tuples to keep C set values
    E = Array{Tuple{Int64, Int64}}(undef,m) # Tuples representing existing edges
    W = zeros(Int64,(n,n)) # Adjacency matrix with weights representing the G(V,A) graph

    # Reads the pair contained in the C set and places then in an array of tuples
    # Each element (a_i,b_i) from the array C are a element from the C set
    for (i, pair) in enumerate(data[3:(2+c)])
        C[i] = Tuple(map(x->parse(Int64,x), split(pair)))
    end

    # Reads the edges presented in the input and its respectives weights
    # The matrix is accesed by W[i,j] = w, which means we are accessing the
    # directed edge, from i to j, with weight w (if w=0 theres no such edge) 
    for (i,edge) in enumerate(data[(3+c):(2+c+m)])
        (v,u,w_vu) = map(x->parse(Int64,x), split(edge))
        E[i] = (v,u)
        W[v,u] = w_vu
    end

    # Returns the structured data
    (n,m,c,s,t,C,E,W)
end

# MODEL: building and execution block
let
    # MODEL BUILDING
    gt54 = Model(solver=GurobiSolver(TimeLimit=TL))

    # Setting variables
    @variable(gt54, x[1:n], Bin)    # Represents if vertex i is in the current path
    @variable(gt54, e[E], Bin)      # Represents if edge (i,j) is in the current path
    
    # objective function: minimize path wheight (sum of the edges weights in the path)
    @objective(gt54, Min, sum(W[i,j]*e[(i,j)] for (i,j) in E))

    # CONSTRAINTS
    # The starting and ending vertices must be a part of the solution
    @constraint(gt54, x[s] == 1)
    @constraint(gt54, x[t] == 1)

    # Pairs contained in C must have at most one vertex in the solution
    for (a_i, b_i) in C
        @constraint(gt54, x[a_i]+x[b_i] <= 1)
    end

    # If an edge is a part of the solution, so are its vertices
    for (i,j) in E
        @constraint(gt54, x[i]+x[j]-2*e[(i,j)] >= 0)
    end
    
    # Iterate through every vertex and apply constraints
    for i = 1:n
        # For every vertice (except t), if its a part of the solution, exit degree must be 1
        if i != t 
            @constraint(gt54, x[i] - sum(e[(i,u)] for u = 1:n if W[i,u] != 0) == 0)
        end

        # For every vertice (except s), if its a part of the solution, entry degree must be 1
        if i != s
            @constraint(gt54, x[i] - sum(e[(v,i)] for v = 1:n if W[v,i] != 0) == 0)
        end
    end
    
    status = solve(gt54)
   
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

    println("Número de nós explorados: ", getnodecount(gt54::Model))
    D = getobjbound(gt54::Model)
    Pi = getobjectivevalue(gt54::Model)
    @printf("Melhor limitante dual: %.2f\n", D)
    @printf("Melhor limitante primal: %.2f\n", Pi)
    Gap = (abs( D - Pi )/Pi)*100
    @printf("Gap de otimalidade: %.2f\n", Gap)
    @printf("Tempo de execução: %.2f\n", getsolvetime(gt54::Model))
    
end # end model building and execution
