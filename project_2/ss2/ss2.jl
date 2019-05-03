# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# VARIABLES DESCRIPTION
# nt = Amount of tasks in instance (|T| or |D|)
# T  = Set containing times for the tasks durations
# D  = Set containing times for the tasks deadlines
# S  = Set containing precedence relations in pairs ((i,j)=>task i before j)
# s  = Amount of precedence relations in instance (|S|)

# Importing packages
using JuMP, Gurobi, Printf
nome_arq = "Instancias/ss2."*ARGS[1]*".instance"

# Time Limit
if length(ARGS) > 1
  TL = parse(Int64, ARGS[2])
else
  TL = 100000
end

# Input data processing and representation
nt,s,S,T,D = open(nome_arq) do file
    data = readlines(file) # Reads whole input line by line
    (nt,s) = map(x->parse(Int64,x), split(data[1]))
    S = Array{Tuple{Int64, Int64}}(undef,s) # Data representation for pairs in S
    T = Array{Int64}(undef,nt) # Data representation for task duration times
    D = Array{Int64}(undef,nt) # Data representation for task deadline times

    # Reads tasks and places duration and deadline in a list of tuples
    for (i, times) in enumerate(data[2:(1+nt)])
        (T[i],D[i]) = map(x->parse(Int64,x), split(times))
    end

    # Read S pairs and alocate them in a list of tuples
    for (i, pair) in enumerate(data[(2+nt):(1+nt+s)])
        S[i] = Tuple(map(x->parse(Int64,x), split(pair)))
    end

    # Return structured input
    (nt,s,S,T,D)
end

# Start model building and execution
let
    # Create combination (i,j)|N X N for ordering variables removing cases where
    # i == j, since such cases are incoherent for the variables purpose. 
    ordering = Array{Tuple{Int64,Int64}}(undef,nt*nt - nt)
    k = 1
    for i = 1:nt
        for j = 1:nt
            if i != j
                ordering[k] = (i,j)
                k += 1
            end
        end
    end

    # Creating model
    ss2 = Model(solver=GurobiSolver(TimeLimit=TL))
    # Setting variables
    @variable(ss2, y[ordering], Bin)
    @variable(ss2, sig[1:nt] >= 0, Int) # Ensures non-negative start times
    @variable(ss2, x[1:nt], Bin)
    # Setting constant big M
    M = sum(T)
    # Objective function
    @objective(ss2, Min, sum(x))

    # CONSTRAINTS
    # Precedence conditions: if i before j, then j not before i. 
    for (i,j) in ordering 
        @constraint(ss2, y[(i,j)] + y[(j,i)] == 1)
    end
    # Precedence imposed by S set
    for pair in S
        @constraint(ss2, y[pair] ==  1)
    end
    # Ensures ordering between tasks without overlaps
    for (i,j) in ordering
        @constraint(ss2, sig[i] + T[i] <= sig[j] + (1 - y[(i,j)])*M)
    end
    # Ensures that delayed tasks are tagged as such
    for i = 1:nt
        @constraint(ss2, sig[i] + T[i] <= D[i] + M*x[i])
    end

    status = solve(ss2)

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

    println("Número de nós explorados: ", getnodecount(ss2::Model))
    D_x = getobjbound(ss2::Model)
    Pi = getobjectivevalue(ss2::Model)
    @printf("Melhor limitante dual: %.2f\n", D_x)
    @printf("Melhor limitante primal: %.2f\n", Pi)
    Gap = (abs( D_x - Pi )/Pi)*100
    @printf("Gap de otimalidade: %.2f\n", Gap)
    @printf("Tempo de execução: %.2f\n", getsolvetime(ss2::Model))

end # End model building and execution