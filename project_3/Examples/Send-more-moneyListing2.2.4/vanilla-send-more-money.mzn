
%---------------------------------------------------------------------
% Modelo CP para o SEND+MORE=MONEY: versão vanilla
% Preparado por: Cid C. de Souza - IC-Unicamp
% Data: 20190424
%---------------------------------------------------------------------

% declaração de tipos enumerados e conjuntos
enum Letras = { S, E, N, D, M, O, R, Y };
set of int: Digitos = 0..9;

% variávies para as letras
array[Letras] of var Digitos: valor;

% variáveis para os "vai uns"
array[1..4] of var 0..1: carry; 

% restrições: duas letras possuem valores diferentes
constraint forall(i, j in Letras where i < j)(
    valor[i] !=valor[j]);

% retrições: os valores de S e M não podem ser nulos
constraint valor[S] != 0; constraint valor[M] != 0;

% retrição: os valores de M e do carry[4] são iguais
constraint valor[M] = carry[4];

% restrições que garantem a soma correta: SEND+MORE=MONEY
constraint carry[3] + valor[S] + valor[M] = valor[O] + 10 * carry[4];
constraint carry[2] + valor[E] + valor[O] = valor[N] + 10 * carry[3];
constraint carry[1] + valor[N] + valor[R] = valor[E] + 10 * carry[2];
constraint valor[D] + valor[E] = valor[Y] + 10 * carry[1];

solve satisfy;
output ["\n    \(valor[S])\(valor[E])\(valor[N])\(valor[D])\n",
" +  \(valor[M])\(valor[O])\(valor[R])\(valor[E])\n",
" -------\n",
" = \(valor[M])\(valor[O])\(valor[N])\(valor[E])\(valor[Y])\n\n"];

