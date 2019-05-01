# Victor Ferreira Ferrari,  RA 187890
# Vinícius Couto Espindola, RA 188115

# K-Coloração de grafos para k mínimo, PLI

# Importando pacotes
using JuMP, Gurobi, Printf

nome_arq = "Instancias/mn27."*ARGS[1]*".instance"

# Debug
println(nome_arq)

f = open(nome_arq, "r")

let


close(f)

end # Fim do bloco let
