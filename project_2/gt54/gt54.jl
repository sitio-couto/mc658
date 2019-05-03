# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# VARIABLES DESCRIPTION
# n = Amount of nodes in instance
# m = Amount of edges in instance| c = |C|)
# s = Starting vertex
# t = Ending vertex
# c = Cardinality of C set
# C = C set (vertex pairs restrictions) represented by an array of tuples
# W = Adjacency matrix with wheights representing the instance graph 

# Importing packages
using JuMP, Gurobi, Printf
file_name = "Instancias/gt54."*ARGS[1]*".instance"

# Time Limit
if length(ARGS) > 1
    TL = parse(Int64, ARGS[2])
else
    TL = 100000
end

# Input data processing and representation
n,m,c,s,t,C,W = open(file_name) do file
    data = readlines(file) # Reads whole input line by line
    (n,c,m) = map(x->parse(Int64,x), split(data[1]))
    (s,t) = map(x->parse(Int64,x), split(data[2]))
    C = Array{Tuple{Int64, Int64}}(undef,c) # Tuples to keep C set values
    W = zeros(Int64,(n,n)) # Adjacency matrix with wheights representing the G(V,A) graph

    # Reads the pair contained in the C set and places then in an array of tuples
    # Each element (a_i,b_i) from the array C are a element from the C set
    for (i, pair) in enumerate(data[3:(2+c)])
        C[i] = Tuple(map(x->parse(Int64,x), split(pair)))
    end

    # Reads the edges presented in the input and its respectives weights
    # The matrix is accesed by W[i,j] = w, which means we are accessing the
    # edge from i to j with weight w (if w=0 theres no (i,j) edge) 
    for edge in data[(3+c):(2+c+m)]
        (u,v,w_uv) = map(x->parse(Int64,x), split(edge))
        W[u,v] = w_uv
    end

    # Returns the structured data
    (n,m,c,s,t,C,W)
end

# Start model build and execution
let
    # For all existing edge, create a variable e_ij
    edges = Array{Tuple{Int64,Int64}}(undef,m)
    i = 1
    for u = 1:n
        for v = 1:n
            if W[u,v] != 0
                edges[i] = (u,v)
                i+=1
            end
        end
    end
  
    # Creating model
    gt54 = Model(solver=GurobiSolver(TimeLimit=TL))
    # Setting variables
    @variable(gt54, x[1:n], Bin)        # Represents if vertex i is in the current path
    @variable(gt54, e[i in edges], Bin) # Represents if edge (i,j) is in the current path
    # objective function: minimize path wheight (sum of the edges wheights in the path)
    @objective(gt54, Min, sum(W[i,j]*e[(i,j)] for (i,j) in edges))

    # CONSTRAINTS
    # The starting and ending vertices must be a part of the solution
    @constraint(gt54, x[s] == 1)
    @constraint(gt54, x[t] == 1)

    # Pairs contained in C must not be a part of the solution
    for (a_i, b_i) in C
        @constraint(gt54, x[a_i]+x[b_i] <= 1)
    end

    # If an edge is a part of the solution, so are its vertices
    for (i,j) in edges
        @constraint(gt54, x[i]+x[j]-2*e[(i,j)] >= 0)
    end

    # Iterate through every vertex and apply constraints
    for i = 1:n
        # For every vertice (except t), if its a part of the solution, exit degree must be 1
        if i != t 
            @constraint(gt54, x[i] - sum(e[(u,v)] for (u,v) in edges if u==i) == 0)
        end

        # For every vertice (except s), if its a part of the solution, entry degree must be 1
        if i != s
            @constraint(gt54, x[i] - sum(e[(u,v)] for (u,v) in edges if v==i) == 0)
        end
    end
    
    status = solve(gt54)
   
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

    println("Número de nós explorados: ", getnodecount(gt54::Model))
    D = getobjbound(gt54::Model)
    Pi = getobjectivevalue(gt54::Model)
    @printf("Melhor limitante dual: %.2f\n", D)
    @printf("Melhor limitante primal: %.2f\n", Pi)
    Gap = (abs( D - Pi )/Pi)*100
    @printf("Gap de otimalidade: %.2f\n", Gap)
    @printf("Tempo de execução: %.2f\n", getsolvetime(gt54::Model))
    
end # end model building and execution
