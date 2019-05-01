# --------------------------------------------------------------------
# Problema: clique máxima em um grafo
# Exemplo usando Julia, a biblioteca JuMP e o resolvedor PLI Gurobi 
#                                                        (ou CPLEX)
# Preparado por Cid C. de Souza [IC-Unicamp-2019s1]
# --------------------------------------------------------------------

# Importando os pacotes para fazer a otimização
#using JuMP, Gurobi, Printf
using JuMP, CPLEX, Printf

# --------------------------------------------------------------------
# função que lê o nome do arquivo da linha de comando
# --------------------------------------------------------------------
function Le_nome_arquivo(args)
    @show args
    #println(args[1])
end
# --------------------------------------------------------------------

Le_nome_arquivo(ARGS)
nome_arq="Instancias/grafo"*ARGS[1]*".txt"
println(nome_arq)

f = open(nome_arq,"r")

# ----------------------------------------------------------------------------------------
# IMPORTANTE: o programa supõe que o arquivo de entrada é dado no seguinte formato
#    linha 1...........: dois inteiros n e m (número de nós e arestas respectivamente)
#    m linhas seguintes: dois inteiros com as extremidades de uma aresta (cada linha)
# ----------------------------------------------------------------------------------------

# ----------------------------------------------------------------------------------------
# IMPORTANTE: sem este "let", as variáveis de fora do loop não ficam definidas dentro dele
# ----------------------------------------------------------------------------------------
let

# Estas variáveis serão usadas no loop e precisam ser declaradas aqui fora para
# que eu possa alterá-las no while. Do contrário, elas ficam locais no while

nl=1; ne=0; n=0; m=0; # => nl= número de linhas processadas; ne=número de arestas processadas
head=zeros(Int64,(1,1))
tail=zeros(Int64,(1,1))

while !eof(f)
   println(">>> Nova iteração do while: nl=$nl ne=$ne")
   line=readline(f) 
   println("    linha $(nl) sendo processada:")
   if (nl==1) # => processa linha 1 contendo os valores de n e m
      n, m = [parse(Int, x) for x in split(line)]
      head=zeros(Int64,(1,m))   # inicializa vetor de heads
      tail=zeros(Int64,(1,m))   # inicializa vetor de tails
      println("n=$n, m=$m, ne=$ne\n tail = $tail\n head = $head\n ----------\n")
      ne=1
   else
      head[ne], tail[ne] = [parse(Int64, x) for x in split(line)]
      println("    head[$(ne)]=$(head[ne]), tail[$(ne)]=$(tail[ne])")
      ne += 1
   end # => if
   nl = nl+1
end # => while

println("n = $n")
println("m= $m")
println("head = $head")
println("tail = $tail")

close(f) # => fecha arquivo de entrada

Iarestas=1:m  # => conjunto de índices das arestas
Ivertices=1:n # => conjunto de índices dos vértices

# => Constrói conjunto de arestas (array de tuplas)
Arestas = Array{Tuple{Int64, Int64}}(undef,m)
for i in 1:m
  Arestas[i] = (head[i], tail[i])
end

# --------------------------------------------------------------------
# Modelo para o problema da clique máxima em grafos
# --------------------------------------------------------------------

# Preparando o modelo
#CLIQUE = Model(solver=GurobiSolver())
CLIQUE = Model(solver=CplexSolver())

# definindo as variáveis
@variable(CLIQUE, x[i in Ivertices], Bin)

# função objetivo (FO)
@objective(CLIQUE, Max, sum( x[i] for i in Ivertices)  )

# restrições: dois vértices NÃO adjacentes não podem estar na CLIQUE 
#             simultaneamente
for i in 1:(n-1)
  for j in (i+1):n
    if !((i,j) in Arestas)
      @constraint(CLIQUE, x[i]+x[j] <= 1)
    end
  end
end

print(CLIQUE)
status = solve(CLIQUE)
println("The solution status is: $status")
obj = getobjectivevalue(CLIQUE)
x_star = getvalue(x)

println("The optimal objective function value is = $obj")
println(x_star)

print("Vértices na CLIQUE: ")
for i in Ivertices
  if x_star[i]==1 
    print("$i ")
  end
end
println()

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

println("Número de nós explorados: ", getnodecount(CLIQUE::Model))
D = getobjbound(CLIQUE::Model)
P = getobjectivevalue(CLIQUE::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
Gap = (abs( D - P )/P)*100
@printf("Gap de otimalidade: %.2f\n", Gap)
@printf("Tempo de execução: %.2f\n", getsolvetime(CLIQUE::Model))

end  # => fim do bloco "let"
