%---------------------------------------------------------------------
% Modelo (vanilla) do Bin Packing com surrogate constraint
% Preparado por: Cid C. de Souza
% Adaptado do exemplo do knapsack do Minizinc (listing 2.2.13)
% IC-UNICAMP ::  20190429
%---------------------------------------------------------------------

set of int: ITEM;
set of int: BINS;

array[ITEM] of int: weights;
array[BINS] of int: capacity;

% multiplicadores das restrições de capacidade (2)
array[BINS] of int: alpha;

% variáveis de decisão: item em um bin ou não
array[ITEM,BINS] of var 0..1: x;

% restrição 1: um item tem que estar em um bin
constraint 
  forall (i in ITEM) (
     sum (j in BINS) (x[i,j]) = 1
  );

% restrição 2: a soma dos pesos  dos itens em um  bin devem respeitar
%              sua capacidade
constraint
  forall (j in BINS) ( 
     sum (i in ITEM) (weights[i]*x[i,j]) <= capacity[j]
  );

% restrição 3: surrogate
constraint
     sum(i in ITEM, j in BINS) (alpha[j]*weights[i]*x[i,j]) <= 
                          sum(j in BINS) (alpha[j]*capacity[j]);

solve satisfy;

output[if (fix(x[i,j])>-1) /\ (i==1) then "*** Bin \(j): (\(capacity[j]))\n" else "" endif ++ 
       if fix(x[i,j])==1 then "\(i) (\(weights[i]))\n" else "" endif | j in BINS, i in ITEM ];

