# --------------------------------------------------------------------
# Problema: Flowshop em 2 máquinas, minimizar soma dos tempos de fim
#           na máquina 2
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

#   ------------------------------------------------------------------
# Início do programa: 
#   ------------------------------------------------------------------

#    Leitura dos dados  de entrada -----------------------------------
#    Formato do arquivo:  primeira linha tem um inteiro  com número de
#    tarefas. Demais  n linha  tem uma tripla  de inteiros:  índice da
#    tarefa, duração  da tarefa na  máquina 1  e duração da  tarefa na
#    máquina 2.

Le_nome_arquivo(ARGS)
nome_arq="Instances/flow"*ARGS[1]*".txt"
println(nome_arq)
f = open(nome_arq,"r")

#    Uso o "let" para considerar um único bloco e não ter problema 
#    com escopo de variáveis em laços
let

#    processa a primeira inha contendo o valor de n=no. tarefas
n=parse(Int,readline(f))

#    declara os vetores que vão conter as durações das tarefas nas máquinas
d1=zeros(Int64,(1,n))
d2=zeros(Int64,(1,n))

#    processa as demais linhas obtendo as durações das tarefas na s máquinas
for i in 1:n
   a, d1[i], d2[i] = [parse(Int64, x) for x in split(readline(f))]
end

println("n=$n")
println("d1 = $d1")
println("d2 = $d2")

#    fecha o arquivo de dados 
close(f)

# ====================================================================
#    criação do modelo matemático, descrito no arquivo 
#    ../Gnumeric-PLI-Solver/Flowshop/flowshop-modelo.txt
# ====================================================================

#    preliminares

#    Constrói conjunto de pares ordenados de tarefas
m=n*(n-1)  # número de pares ordenados (exclui pares do tipo (i,i))
Pares = Array{Tuple{Int64, Int64}}(undef,m)
k=1
for i in 1:n
   for j in 1:n
      if (i != j)
         Pares[k] = (i,j)
         k=k+1
      end
   end
end
#println("$Pares") # imprime pares da tupla: debug only!

#    Calcula BM e MM (valores suficientemente grandes)
M=0  # soma duração de todas tarefas na máquina 1
MM=0 # soma duração de todas tarefas nas duas máquinas
for i in 1:n 
   M=M+d1[i]
   MM=MM+d2[i]
end
MM=M+MM

#    Preparando o modelo
# FLOW = Model(solver=GurobiSolver())
FLOW = Model(solver=CplexSolver())

#    definindo as variáveis
@variable(FLOW, f1[1:n], Int)
@variable(FLOW, f2[1:n], Int)
@variable(FLOW, x[e in Pares], Bin)

#    função objetivo
@objective(FLOW, Min, sum(f2[i] for i in 1:n) )

#    restrições

#  R1:  tempo de término da  tarefa j na  máquina 1 deve ser  maior ou
#       igual à  soma do  tempo de  término da tarefa  i quando  esta a
#       precede com a sua própria duração  na máquina 1.  A restrição é
#       redundante se i não precede j.
for e in Pares
   i=e[1]
   j=e[2]
    @constraint(FLOW,f1[j]-f1[i]-(d1[j]+M)*x[e] >= -M)
end

#  R2:  tempo de término da  tarefa j na  máquina 1 deve ser  menor ou
#       igual ao  tempo de término menos  a duração da tarefa  i quando
#       esta a sucede na máquina 1.   A restrição é redundante se i não
#       sucede j.
for e in Pares
   i=e[1]
   j=e[2]
   f=(j,i)  # par oposto ao par de e
   @constraint(FLOW,f1[j]-f1[i]+(d1[i]+M)*x[f] <= M)
end

#  R3:  tempo de término da  tarefa j na  máquina 2 deve ser  maior ou
#       igual ao  tempo de término  da tarefa  i quando esta  a precede
#       mais  a  sua  própria  duração  na máquina  2.  A  restrição  é
#       redundante se i não sucede j.
for e in Pares
   i=e[1]
   j=e[2]
    @constraint(FLOW,f2[j]-f2[i]-(d2[j]+MM)*x[e] >= -MM)
end

#  R4:  tempo de término da  tarefa j na  máquina 2 deve ser  maior ou
#       igual ao seu  tempo de término na máquina 1  mais a sua duração
#       na máquina 2.
for j in 1:n
   @constraint(FLOW, f2[j] - f1[j] >= d2[j])
end
   
# R5: as tarefas i e j não podem preceder uma à outra simultaneamente:
for i in 1:(n-1)
   for j in (i+1):n
      f=(i,j)
      g=(j,i)
      @constraint(FLOW, x[f]+x[g] <= 1)
   end
end		

# R7: limites superiores para variáveis
for j in 1:n
   @constraint(FLOW, f1[j] <= M)
   @constraint(FLOW, f2[j] <= MM)
end

#  R8: toda tarefa só pode ter no máximo uma antecessora imediata 
for j in 1:n
   @constraint(FLOW, sum(x[e] for e in  Pares if e[2]==j) <= 1)
end

#  R9: toda tarefa só pode ter no máximo uma sucessora imediata 
for j in 1:n
   @constraint(FLOW, sum(x[e] for e in  Pares if e[1]==j) <= 1)
end

#  R10:  o número total  de relações de  precedência será o  número de
#        tarefas menos 1.
@constraint(FLOW, sum(x[e] for e in Pares)  == n-1)

#  R11: para todo j \in N, o tempo de término de j na máquina 1
#       deve ser maior que o seu tempo de processamento naquela 
#       máquina
for j in 1:n
   @constraint(FLOW, f1[j] - d1[j] >= 0)
end

#    imprime e resolve o modelo
print(FLOW)
status = solve(FLOW)
println("Status da solução: $status")
obj = getobjectivevalue(FLOW)
f1_star = getvalue(f1)
f2_star = getvalue(f2)
x_star  = getvalue(x)

#    imprime resultados
println("valor ótimo = $obj")
println(f1_star)
println(f2_star)
for i in 1:n
   @printf("%3d:",i)
   for j in 1:n
      if (i != j) 
         e=(i,j)
         @printf("%2d",x_star[e])
      else
         @printf(" N")
      end
   end
   println()
end

# --------------------------------------------------------------------

# relatório estatístico
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

println("Número de nós explorados: ", getnodecount(FLOW::Model))
D = getobjbound(FLOW::Model)
P = getobjectivevalue(FLOW::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
Gap = (abs( D - P )/P)*100
@printf("Gap de otimalidade: %.2f\n", Gap)
@printf("Tempo de execução: %.2f\n", getsolvetime(FLOW::Model))

end  # => fim do bloco "let"



   

