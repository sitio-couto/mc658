# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# Importing packages
using JuMP, Gurobi, Printf

nome_arq = "Instancias/ss2."*ARGS[1]*".instance"

# Input data processing and representation
n,s,S,T,D = open(nome_arq) do file
    data = readlines(file) # Reads whole input line by line
    (n,s) = map(x->parse(Int64,x), split(data[1]))
    S = Array{Tuple{Int64, Int64}}(undef,s) # Data representation for pairs in S
    T = Array{Int64}(undef,n) # Data representation for task duration times
    D = Array{Int64}(undef,n) # Data representation for task deadline times

    # Reads tasks and places duration and deadline in a list of tuples
    for (i, times) in enumerate(data[2:(1+n)])
        (T[i],D[i]) = map(x->parse(Int64,x), split(times))
    end

    # Read S pairs and alocate them in a list of tuples
    for (i, pair) in enumerate(data[(2+n):(1+n+s)])
        S[i] = Tuple(map(x->parse(Int64,x), split(pair)))
    end

    # Return structured input
    (n,s,S,T,D)
end

# Checking input
# print("$n|$s\n")
# display([T D])
# print("\n")
# display(S)
# print("\n")

let
    # Create combination N X N for ordering variables
    ordering = Array{Tuple{Int64,Int64}}(undef,n*n - n)
    k = 1
    for i = 1:n
        for j = 1:n
            if i != j
                ordering[k] = (i,j)
                k += 1
            end
        end
    end

    # Creating model
    ss2 = Model(solver=GurobiSolver())
    # Setting variables
    @variable(ss2, y[pair in ordering], Bin)
    @variable(ss2, sig[1:n], Int)
    @variable(ss2, x[1:n], Bin)
    # Setting constant big M
    M = 1 + sum(T)
    # Objective function
    @objective(ss2, Min, sum(x))

    # CONSTRAINTS
    # Precedence conditions: if i before j, then j not before i. 
    for (i,j) in ordering 
        @constraint(ss2, y[(i,j)] + y[(j,i)] == 1)
    end
    # Precedence imposed by S set
    for pair in S
        @constraint(ss2, y[pair] == 1)
    end
    # Ensures ordering between tasks without overlaps
    for (i,j) in ordering
        @constraint(ss2, sig[i] + T[i] <= sig[j] + (1 - y[(i,j)]*M))
    end
    # For every task i applies constraint
    for i = 1:n
        # Ensures non-negative start times
        @constraint(ss2, sig[i] >= 0) 
        # Ensures that delayed tasks are tagged as such
        @constraint(ss2, sig[i] + T[i] <= D[i] + M*x[i])
    end


end # Fim do bloco let
