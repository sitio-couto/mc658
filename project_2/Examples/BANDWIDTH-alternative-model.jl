# --------------------------------------------------------------------
# Problema: minimizar a largura de banda de um grafo
# Exemplo usando Julia, a biblioteca JuMP e o resolvedor PLI Gurobi 
#                                                        (ou CPLEX)
# Preparado por Cid C. de Souza [IC-Unicamp-2019s1]
#
# IMPORTANTE:  este  modelo  é  uma   alteração  do  modelo  que  está
#              implementado no arquivo  BANDWIDTH-model.jl, onde foram
#              eliminadas  variáveis supérfluas  referentes a  rótulos
#              nos vértices e nas arestas. Não melhora muito! :(
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
nome_arq="Instances/grafo"*ARGS[1]*".txt"
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
Labels=1:n   # => conjunto dos rótulos possíveis

# => Constrói conjunto de arestas (array de tuplas)
Arestas = Array{Tuple{Int64, Int64}}(undef,m)
for i in 1:m
  Arestas[i] = (head[i], tail[i])
end

# --------------------------------------------------------------------
# Modelo para o problema de minimizar a largura de banda de um grafo
# --------------------------------------------------------------------

# tempo limite de computação (em segundos)
TL = 20
# Preparando o modelo
#BAND = Model(solver=GurobiSolver(TimeLimit=TL))
BAND = Model(solver=CplexSolver(CPX_PARAM_TILIM=TL))

# definindo as variáveis
#@variable(BAND, 1 <= u[j in Ivertices] <= n, Int)               # => label do vértice
@variable(BAND, x[i in Ivertices, j in Labels], Bin)  # => vértice i recebe label j
#@variable(BAND, v[e in Arestas], Int)                 # => label da aresta
@variable(BAND, z, Int)                               # => bandwitdth

# função objetivo (FO)
@objective(BAND, Min, z )

# restrições (1): todo vértice recebe um label
for i in Ivertices
  @constraint(BAND, sum(x[i,j] for j in Labels) == 1)
end

# restrições (2): todo label é associado a exatamente um vértice
for j in Labels
  @constraint(BAND, sum(x[i,j] for i in Ivertices) == 1)
end

# restrições (3): obtém o label do vértice
#for i in Ivertices
#  @constraint(BAND, sum(j*x[i,j] for j in Labels) == u[i])
#end

# restrições (4): limita inferiormente o label de uma aresta
for (i,j) in Arestas
  @constraint(BAND, z >= sum(k*x[i,k] for k in Labels)-sum(k*x[j,k] for k in Labels) )
  @constraint(BAND, z >= sum(k*x[j,k] for k in Labels)-sum(k*x[i,k] for k in Labels) )
end

# restrições (5): limita inferiormente o bandwidth
#for (i,j) in Arestas
#  @constraint(BAND, z >= v[(i,j)])
#end

#println("n=$n")
# restrição (6): soma dos rótulos dos vértices
#@constraint(BAND, sum(u[j] for j in Ivertices) == Int(n*(n+1)/2) )

print(BAND)
status = solve(BAND)
println("Status da otimização: $status")
obj = getobjectivevalue(BAND)
#u_star= getvalue(u)
#v_star= getvalue(v)
x_star= getvalue(x)

println("Valor da FO = $obj")

print("Rótulos dos vértices: ")
for i in Ivertices
   for k in Labels
      if x_star[i,k] > 0.5
         @printf("%d (%d); ",i,k)
      end
   end
end
println()

#println("Rótulos das arestas: ")
#for (i,j) in Arestas
#  @printf("(%d,%d) (%d); \n",i,j,round(v_star[(i,j)]))  
#end
#println()

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

println("Número de nós explorados: ", getnodecount(BAND::Model))
D = getobjbound(BAND::Model)
P = getobjectivevalue(BAND::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
Gap = (abs( D - P )/P)*100
@printf("Gap de otimalidade: %.2f\n", Gap)
@printf("Tempo de execução: %.2f\n", getsolvetime(BAND::Model))

end  # => fim do bloco "let"

