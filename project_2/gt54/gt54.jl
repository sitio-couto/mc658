# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# Importando pacotes
using JuMP, Gurobi, Printf

nome_arq = "Instancias/gt54."*ARGS[1]*".instance"

# Leitura da entrada e montagem da representacao dos dados
n,c,m,s,t,C,P = open(nome_arq) do file
    data = readlines(file) # Le a entrada inteira e pariciona em linhas
    (n,c,m) = map(x->parse(Int64,x), split(data[1]))
    (s,t) = map(x->parse(Int64,x), split(data[2]))
    C = Array{Tuple{Int64, Int64}}(undef,c) # Estrutura para armazenar os pares de C
    P = zeros(Int64,(n,n)) # Estrutura para representar o grafo G(V,A)

    # Le os pares contidos em C e os aloca em uma matriz C = (|C|,2)
    # Assim temos que um par (ai,bi) de c sao os elementos da linha i de C
    for (i, pair) in enumerate(data[3:(2+c)])
        C[i] = Tuple(map(x->parse(Int64,x), split(pair)))
    end

    # Le as arestas presentes no grafo e seus respectivos pesos
    # Usa os vertices para indexar a matrix de adjacencia P, de forma
    # que P[i,j] guarde o custo da aresta (i,j), ou zero se inexistente
    for edge in data[(3+c):(2+c+m)]
        (u,v,c_uv) = map(x->parse(Int64,x), split(edge))
        P[u,v] = c_uv
    end

    # Retorna os valores estruturados
    (n,c,m,s,t,C,P)
end

# Checking input
# print("$n|$c|$m\n$s|$t\n")
# display(C)
# print("\n")
# display(P)
# print("\n")

# Start model build and execution
let
    # For all existing edge, create a variable e_ij
    edges = Array{Tuple{Int64,Int64}}(undef,m)
    i = 1
    for u = 1:n
        for v = 1:n
            if P[u,v] != 0
                edges[i] = (u,v)
                i+=1
            end
        end
    end
  
    # Creating model
    gt54 = Model(solver=GurobiSolver())
    # Setting variables
    @variable(gt54, x[1:n], Bin)
    @variable(gt54, e[i in edges], Bin)
    # objective function
    @objective(gt54, Min, sum(P[i,j]*e[(i,j)] for (i,j) in edges))

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

    print(gt54)
    status = solve(gt54)
    println("The solution status is: $status")
    obj = getobjectivevalue(gt54)
    println("The optimal objective function value is = $obj")
    x_star = getvalue(x)
    e_star = getvalue(e)
    println("vertices = {$x_star}")
    println("edges = {$e_star}")

end # end model building and execution
