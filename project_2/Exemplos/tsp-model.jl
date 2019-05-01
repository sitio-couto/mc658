# --------------------------------------------------------------------
# Problema: TSP em um grafo completo
#           modelo de Miller, Tucker e Zemlin (MTZ)
# Exemplo usando Julia, a biblioteca JuMP e o resolvedor PLI Gurobi 
#                                                        (ou CPLEX)
#
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
# função que retorna o índice de uma posição em uma matriz
# triangular superior de ordem n dados a linha "a" e a coluna "b".
# Nota: não há elementos na diagonal e a contagem começa da
#       linha 1 coluna 2 com o índice 1, incrementando de 1 em 1 
#       da esquerda para direitam de cima para baixo
# --------------------------------------------------------------------
function calcula_idx(a::Int64, b::Int64, n::Int64)
   # a linha deve ser sempre <= a coluna
   w = 0
   if (a > b) 
      w = b
      b = a 
      a = w
   end
   w = (a-1)*n-floor(Int,((a-1)*a)/2)+(b-a)
   return w
end
# --------------------------------------------------------------------

# --------------------------------------------------------------------
# função que retorna a linha "i" e a coluna "j" de uma matriz
# triangular superior de ordem n dado o índice da posição.
# Nota: não há elementos na diagonal e a contagem começa da
#       linha 1 coluna 2 com o índice 1, incrementando de 1 em 1 
#       da esquerda para direitam de cima para baixo
# --------------------------------------------------------------------
function calcula_lincol(m::Int64, n::Int64)
   # a linha deve ser sempre <= que a coluna
   i = 1
   s = n-1  # acumula número de elementos até a linha i
   while s < m
      i = i + 1
      s = s + (n-i)  #llinha i tem (n-i) elementos 
   end
   # s é o índice do último elemento da linha i
   # A fórmula abaixo encontra a coluna j correspondente ao índice m
   j = n - s + m 

   # sempre retorna a linha menor que a coluna (triangular superior)
   if i < j
      return i, j
   else 
      return j, i
   end
end
# --------------------------------------------------------------------

Le_nome_arquivo(ARGS)
nome_arq="Instancias/k"*ARGS[1]*".txt"
println(nome_arq)

f = open(nome_arq,"r")

# ----------------------------------------------------------------------------------------
# IMPORTANTE: o programa supõe que o arquivo de entrada é dado no seguinte formato
#    linha 1...........: um inteiro n  (número de nós do grafo)
#    m linhas seguintes: 3 inteiros com as extremidades de uma aresta e seu peso,
#                        onde m=(n-1)*n/2 
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
peso=zeros(Int64,(1,1))

while !eof(f)
   println(">>> Nova iteração do while: nl=$nl ne=$ne")
   line=readline(f) 
   println("    linha $(nl) sendo processada:")
   if (nl==1) # => processa linha 1 contendo os valores de n e m
      #n = [parse(Int, x) for x in split(line)]
      n=parse(Int,line) # número de vértices
      m=Int(n*(n-1)/2)  # número de arestas no K_n
      head=zeros(Int64,(1,m))   # inicializa vetor de heads
      tail=zeros(Int64,(1,m))   # inicializa vetor de tails
      peso=zeros(Int64,(1,m))   # inicializa vetor de pesos
      println("n=$n, m=$m, ne=$ne\n tail = $tail\n head = $head\n ----------\n")
      ne=1
   else
      head[ne], tail[ne], peso[ne] = [parse(Int64, x) for x in split(line)]
      println("    head[$(ne)]=$(head[ne]), tail[$(ne)]=$(tail[ne]), peso[$(ne)]=$(peso[ne])")
      ne += 1
   end # => if
   nl = nl+1
end # => while

println("n = $n")
println("m= $m")
println("head = $head")
println("tail = $tail")
println("peso = $peso")

close(f) # => fecha arquivo de entrada

Iarestas=1:m  # => conjunto de índices das arestas
Ivertices=1:n # => conjunto de índices dos vértices

# => Constrói conjunto de arestas (array de tuplas)
#    NOTA: no MTZ temos arcos direcionados no lugar
#          das arestas. Portanto, dobra a quantidade!
Arestas = Array{Tuple{Int64, Int64}}(undef,2*m)
k=1
for i in Ivertices
   for j in Ivertices
      if (i != j)
         Arestas[k] = (i,j)
         k=k+1
      end
   end
end
println("$Arestas")

# verificação das funções
for i in Ivertices
   for j in Ivertices
      if (i < j)
         k = calcula_idx(i,j,n)
         a, b = calcula_lincol(k,n)
         if (a!=i) && (b!=j)
            println("ERRO: a=$a, i=$i, b=$b, j=$j")
            exit(10)
         end 
      end
   end
end



#for e in Arestas
#   println("$(e[1]), $(e[2]): ", calcula_idx(e[1],e[2],n))
#end

# --------------------------------------------------------------------
# Modelo MTZ para o TSP
# --------------------------------------------------------------------

# Preparando o modelo
# TSP = Model(solver=GurobiSolver())
TSP = Model(solver=CplexSolver())

# definindo as variáveis
@variable(TSP, x[e in Arestas], Bin)
@variable(TSP, 2 <= u[2:n] <= n, Int)

# função objetivo (FO)
@objective(TSP, Min, sum(peso[calcula_idx(e[1],e[2],n)]*x[e] for e in Arestas))

# restrições: de todo vértice sai uma aresta
for i in Ivertices
  @constraint(TSP, sum(x[e] for e in Arestas if e[1]==i) == 1)
end

# restrições: de todo vértice entra uma aresta
for j in Ivertices
  @constraint(TSP, sum(x[e] for e in Arestas if e[2]==j) == 1)
end

# restrições do MTZ para evitar ciclos
# NOTA: só existem para o caso em que extremidades não
#       são o vértice 1.           
for e in Arestas
    i, j = e 
    if (i != 1) && (j != 1)
       @constraint(TSP, u[i] - u[j] + n*x[e] <= n-1)
    end
end

print(TSP)
status = solve(TSP)
println("The solution status is: $status")
obj = getobjectivevalue(TSP)
x_star = getvalue(x)
u_star = getvalue(u)

println("The optimal objective function value is = $obj")
println(x_star)
println(u_star)

println("Ordem dos vértices no TSP: ")
println("1: 1")
for i in 2:n
    @printf("%d: %d\n",i, u_star[i])
end


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

println("Número de nós explorados: ", getnodecount(TSP::Model))
D = getobjbound(TSP::Model)
P = getobjectivevalue(TSP::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
Gap = (abs( D - P )/P)*100
@printf("Gap de otimalidade: %.2f\n", Gap)
@printf("Tempo de execução: %.2f\n", getsolvetime(TSP::Model))

end  # => fim do bloco "let"

