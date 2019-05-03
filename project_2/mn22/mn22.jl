# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# Given a bipartite graph G(V,U,E), we define the variables:
# E  = Array of edges in bipartite graph G(V,U,E)
# C  = Array of costs for each respective edge in E 
# cv = Cardinality of V (|V| = amount of machines)
# cu = Cardinality of U (|U| = amount of parts)
# m  = Amount of edges between machines and parts (|E|)
# K  = Maximum amount of machines that can fit in a room

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
cv,cu,m,K,E,C = open(file_name) do file
    data = readlines(file)  # Reads whole input line by line
    (cv,cu,m,K) = map(x->parse(Int64,x), split(data[1])) # Reads cardinalities (u,v and m) and bounds (k)
    V = Array{Int64}(undef,cv) # Contains nodes representing machines
    U = Array{Int64}(undef,cu) # Contains nodes representing parts
    E = Array{Tuple{Int64,Int64}}(undef,m) # Contains edges representations
    C = Array{Int64}(undef,m)              # Contains edges costs

    # Reads vertices, edges and costs and places then proper structures
    for (i, edges) in enumerate(data[2:(1+m)])
        (v,u,c_vu) = map(x->parse(Int64,x), split(edges))
        # Adds edge representation and its cost to structures
        E[i] = (v,u) # Tuple (v,u) representing an edge i where "v" is a machine and "u" a part
        C[i] = c_vu  # Cost c of edge i represented in E
    end

    # Return structured input
    (cv,cu,m,K,E,C)
end

# Checking input
println("$cv|$cu|$m|$K")
display([E C])
print("\n")

let 
    # NOTE: in the worst distribution case, there will be one machine per
    # room, therefore the maximum amount of rooms is at most the same as 
    # the amount of machines in the instance. 
    max_r = cv

    # COMBINATIONS FOR THE MODEL
    # Create a combination M x R: all dependence machine-part (i,j) paired with
    # every r room possible.
    count = 1
    lambdas = Array{Tuple{Int64,Int64,Int64}}(undef,m*max_r)
    for (i,j) in E
        for r = 1:max_r
            lambdas[count] = (i,j,r)
            count += 1
        end
    end

    # Create a combination V x R: all possible placements of a machine i in a room r.
    count = 1
    v_ir = Array{Tuple{Int64,Int64}}(undef,cv*max_r)
    for i = 1:cv
        for r = 1:max_r
            v_ir[count] = (i,r)
            count += 1
        end
    end

    # Create a combination U x R: for every possible placement of part j in a room r.
    count = 1
    u_jr = Array{Tuple{Int64,Int64}}(undef,cu*max_r)
    for j = 1:cu
        for r = 1:max_r
            u_jr[count] = (j,r)
            count += 1
        end
    end

    #-----------------------------------------------------------------------------------

    # Creating model
    mn22 = Model(solver=GurobiSolver(TimeLimit=TL))
    
    # Setting variables
    @variable(mn22, v[v_ir], Bin)    # Indicates if machine i is in r room
    @variable(mn22, u[u_jr], Bin)    # Indicates if part j is in r room
    @variable(mn22, l[lambdas], Bin) # Indicates if machine-part (i,j) are in the same room r
    @variable(mn22, t[E], Bin)       # Indicates if machine-part (i,j) share a room
    
    # Objective function: if machine-part pair (i,j) do not share a room, include transport cost.
    @objective(mn22, Min, sum(t[E[i]]*C[i] for i = 1:m))
    
    # CONSTRAINTS
    # Each machine i must be in exactly one room
    for i = 1:cv
        @constraint(mn22, sum(v[(i,r)] for r = 1:max_r) == 1)
    end
    # Each part j must be in exactly one room
    for j = 1:cu
        @constraint(mn22, sum(u[(j,r)] for r = 1:max_r) == 1)
    end
    # Each room r may have at most K machines
    for r = 1:max_r
        @constraint(mn22, sum(v[(i,r)] for i = 1:cv) <= K)
    end
    # Logical "and" operator for the proposition: v_ir and u_jr == l_ijr.
    # Meaning that, if <machine i in room r> and <part j in room r>, then 
    # i and j are in the same room r (l_ijr == 1).
    for (i,j,r) in lambdas
        @constraint(mn22, l[(i,j,r)] <= v[(i,r)])
        @constraint(mn22, l[(i,j,r)] <= u[(j,r)])
        @constraint(mn22, l[(i,j,r)] >= v[(i,r)] + u[(j,r)] - 1)    
    end
    # Variable t (theta) must be the sum of lambdas for a given machine-part
    # dependence, iterating every room r. Such constraint allows t[(i,j)] to
    # indicate if machine i and part j share any of the r rooms.
    for (i,j) in E
        @constraint(mn22, t[(i,j)] == 1 - sum(l[(i,j,r)] for r = 1:max_r))
    end

    status = solve(mn22)
    # print(mn22)
    # val_v = getvalue(v)
    # val_u = getvalue(u)
    # val_l = getvalue(l)
    # val_t = getvalue(t)
    # println("v = $val_v")
    # println("u = $val_u")
    # println("l = $val_l")
    # println("t = $val_t")

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

    println("Número de nós explorados: ", getnodecount(mn22::Model))
    D_x = getobjbound(mn22::Model)
    Pi = getobjectivevalue(mn22::Model)
    @printf("Melhor limitante dual: %.2f\n", D_x)
    @printf("Melhor limitante primal: %.2f\n", Pi)
    Gap = (abs( D_x - Pi )/Pi)*100
    @printf("Gap de otimalidade: %.2f\n", Gap)
    @printf("Tempo de execução: %.2f\n", getsolvetime(mn22::Model))

end 