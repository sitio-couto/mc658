

int: n;
set of int: COLUNAS = 1..n;
set of int: LINHAS = 1..n;  %% DUAL MODELING
array [1..n] of var 1..n: row; % queen of column i is in row row[i]
array [1..n] of var 1..n: col; % queen of row i is in column col[i]

% restrições
constraint %% queens cannot be placed on the same row
    forall(i,j in COLUNAS where i<j) (
       (row[i] != row[j])
    );

% restrições:  DUAL MODELING
constraint %% correspondence between two sets of variables
    forall(i in LINHAS, j in COLUNAS) (
       (row[j]=i <-> col[i]==j)
    );

constraint %% queens cannot be placed on the same diagonal (+45graus)
    forall(i,j in COLUNAS where i<j) (
       (row[i]+i != row[j]+j)
    );

constraint %% queens cannot be placed on the same diagonal (-45graus)
    forall(i,j in COLUNAS where i<j) (
       (row[i]-i != row[j]-j)
    );

solve satisfy;
output [ if fix(row[j]) == i then "Q" else "." endif ++
         if j == n then "\n" else "" endif | i,j in 1..n]


