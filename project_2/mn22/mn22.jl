# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# nv = Amount of machines (|V|)
# nu = Amount of parts (|U|)
# m  = Amount of edges connection parts and machines (|E|)
# K  = Maximum amount of machines that can fit in a room
# G  = Array of edges in bipartite graph G(V,U,E)
# C  = Array of costs for each respective edge in G
# NOTE: the ".X" syntax (using the dot before the operation) applies the operation 'X'
#       by element within the vectors used, removing the need for certain loops.

# Importando pacotes
using JuMP, Gurobi, Printf
file_name = "Instancias/mn22."*ARGS[1]*".instance"

# Time Limit
if length(ARGS) > 1
    TL = parse(Int64, ARGS[2])
else
    TL = 100000
end

# Input data processing and representation
nv,nu,m,K,G,C = open(file_name) do file
    data = readlines(file)  # Reads whole input line by line
    (nv,nu,m,K) = map(x->parse(Int64,x), split(data[1])) # Reads cardinalities (u,v and m) and bounds (k)
    G = Array{Tuple{Int64,Int64}}(undef,m)
    C = Array{Int64}(undef,m)

    # Reads edges and costs and places then in a triple
    for (i, edges) in enumerate(data[2:(1+m)])
        (a,b,c) = map(x->parse(Int64,x), split(edges))
        G[i] = (a,b)    # Tuple (a,b) representing an edge i
        C[i] = c        # Cost c of edge i represented in G
    end

    # Return structured input
    (nv,nu,m,K,G,C)
end

# Checking input
println("$v|$u|$m|$k")
display([G C])
print("\n")

let 
    # Create a combination M x NK: for every dependence machine-part (i,j) pair with
    # every k room possible in the worst distribution case (one machine peer room). 
    count = 1
    lambdas = Array{Tuple{Int64,Int64,Int64}}(undef,m*nk)
    for (i,j) in G
        for k = 1:K
            lambdas[count] = (i,j,k)
            count += 1
        end
    end 

    # Creating model
    mn22 = Model(solver=GurobiSolver(TimeLimit=TL))
    # Setting variables
    @variable(mn22, v[1:nv], Bin)
    @variable(mn22, u[1:nu], Bin)
    @variable(mn22, c[])
end 