
int: n;
set of int: COLUNAS = 1..n;
array [1..n] of var 1..n: q; % queen of column i is in row q[i]

% restrições
constraint %% queens cannot be placed on the same row
    forall(i,j in COLUNAS where i<j) (
       (q[i] != q[j])
    );

constraint %% queens cannot be placed on the same diagonal (+45graus)
    forall(i,j in COLUNAS where i<j) (
       (q[i]+i != q[j]+j)
    );

constraint %% queens cannot be placed on the same diagonal (-45graus)
    forall(i,j in COLUNAS where i<j) (
       (q[i]-i != q[j]-j)
    );

solve satisfy;
output [ if fix(q[j]) == i then "Q" else "." endif ++
         if j == n then "\n" else "" endif | i,j in 1..n]

