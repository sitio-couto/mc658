
include "cumulative.mzn";

enum MOVEIS;
array[MOVEIS] of int: duracao;      % duracao do transporte 
array[MOVEIS] of int: carregadores; % número de carregadores necessários
array[MOVEIS] of int: carrinhos;    % número de carrinhos necessários

int: total_de_carregadores;
int: total_de_carrinhos;
int: total_de_tempo;

% variáveis
array[MOVEIS] of var 0..total_de_tempo: inicio;
var 0..total_de_tempo: fim;

% restrições cumulative
constraint cumulative(inicio, duracao, carregadores, total_de_carregadores);
constraint cumulative(inicio, duracao, carrinhos, total_de_carrinhos);
 
constraint forall(o in MOVEIS)(inicio[o] + duracao[o] <= fim);

% FO: terminar o mais cedo possível (minimizar makespan)
solve minimize fim;

output [ "inicio = \(inicio)\nfim = \(fim)\n"];

