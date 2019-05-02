# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# v = Amount of machines (|V|)
# u = Amount of parts (|U|)
# m = Amount of edges connection parts and machines (|E|)
# k = Maximum amount of machines that can fit in a room
# G = Array of edges in bipartite graph G(V,U,E)
# C = Array of costs for each respective edge in G
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
v,u,m,k,G,C = open(file_name) do file
    data = readlines(file)  # Reads whole input line by line
    (v,u,m,k) = map(x->parse(Int64,x), split(data[1])) # Reads cardinalities (u,v and m) and bounds (k)
    G = Array{Tuple{Int64,Int64}}(undef,m)
    C = Array{Int64}(undef,m)

    # Reads edges and costs and places then in a triple
    for (i, edges) in enumerate(data[2:(1+m)])
        (a,b,c) = map(x->parse(Int64,x), split(edges))
        G[i] = (a,b)    # Tuple (a,b) representing an edge i
        C[i] = c        # Cost c of edge i represented in G
    end

    # Return structured input
    (v,u,m,k,G,C)
end

println("$v|$u|$m|$k")
display([G C])
print("\n")
