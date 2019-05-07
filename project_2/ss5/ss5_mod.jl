# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# PROBLEM [ss5]
# Minimize the total cost due to delayed tasks given durations and deadlines

# VARIABLES DESCRIPTION
# n = Amount of tasks in instance (|T|, |D| or |P|)
# T = Tasks durations (T[i] = t_i <duration of task i>)
# D = Tasks deadlines (D[i] = d_i <deadline of task i>)
# P = Tasks delays cost by time unit (P[i] = p_i <cost of task i delay>)

# Importing packages
using JuMP, Gurobi, Printf
file_name = "Instancias/ss5."*ARGS[1]*".instance"

# Time Limit
if length(ARGS) > 1
    TL = parse(Int64, ARGS[2])
else
    TL = 100000
end

# INPUT: data processing and representation block
n,T,D,P = open(file_name) do file
    data = readlines(file)     # Reads whole input line by line
    n = parse(Int64, data[1])  # Reads number indicating amount of tasks
    T = Array{Int64}(undef,n)  # task duration times data representation
    D = Array{Int64}(undef,n)  # task deadline times data representation
    P = Array{Int64}(undef,n)  # task delay cost data representation

    # Reads tasks and places duration and deadline in a list of tuples
    for (i, times) in enumerate(data[2:(1+n)])
        (T[i],D[i],P[i]) = map(x->parse(Int64,x), split(times))
    end

    # Return structured input
    (n,T,D,P)
end

# MODEL: building and execution block
let
    # COMBINATIONS FOR THE MODEL
    # Create combination (i,j)|N X N for ordering variables removing cases where
    # i == j, since such cases are incoherent for the variables purpose. 
    ordering = Array{Tuple{Int64,Int64}}(undef, 0)
    for i = 1:n
        for j = (i+1):n
            ordering = vcat(ordering, (i,j))
        end
    end

    #--------------------------------------------------------------------

    # MODEL BUILDING
    ss5 = Model(solver=GurobiSolver(TimeLimit=TL))
    # Setting variables
    @variable(ss5, y[ordering], Bin)   # y[(i,j)] indicates if task i comes before j
    @variable(ss5, sig[1:n] >= 0, Int) # sig[i] (non-negative) indicates starting time for task i        
    @variable(ss5, m[1:n] >= 0, Int)   # m[i] (non-negative) indicates the amount of time task i is delayed
    # Setting constant big M
    M = sum(T)
    # Objective function
    @objective(ss5, Min, sum(m .* P))

    # CONSTRAINTS
    # Ensures ordering between tasks without overlaps
    for (i,j) in ordering
        @constraint(ss5, sig[i] + T[i] <= sig[j] + (1 - y[(i,j)])*M)
        @constraint(ss5, sig[j] + T[j] <= sig[i] + y[(i,j)]*M)
    end
    # Forces m to be equal to the delay time (or to 0 if no delay)
    for i = 1:n
        @constraint(ss5, sig[i] + T[i] - D[i] - m[i]<= 0)
    end

    status = solve(ss5)

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

    println("Número de nós explorados: ", getnodecount(ss5::Model))
    D_x = getobjbound(ss5::Model)
    Pi = getobjectivevalue(ss5::Model)
    @printf("Melhor limitante dual: %.2f\n", D_x)
    @printf("Melhor limitante primal: %.2f\n", Pi)
    Gap = (abs( D_x - Pi )/Pi)*100
    @printf("Gap de otimalidade: %.2f\n", Gap)
    @printf("Tempo de execução: %.2f\n", getsolvetime(ss5::Model))

end # End model building and execution