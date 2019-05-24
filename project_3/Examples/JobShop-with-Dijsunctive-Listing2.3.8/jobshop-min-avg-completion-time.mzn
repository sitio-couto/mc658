%---------------------------------------------------------------------
% Job Shop Scheduling Problem:  JSP
% Preparado por Cid C. de Souza -- Instituto de Computação -- UNICAMP
% Adaptado do código disponibilizado no manual do MINIZINC
%---------------------------------------------------------------------

include "disjunctive.mzn";

int: jobs;                                    % # jobs
set of int: JOB = 1..jobs;
int: tasks;                                   % # tasks per job = # machines
set of int: TASK = 1..tasks;
array [JOB,TASK] of int: d;                   % task durations
int: total = sum(i in JOB, j in TASK)(d[i,j]);% total duration
int: digs = ceil(log(10.0,total));            % digits for output
array [JOB,TASK] of var 0..total: s;          % start times
var 0..total: end;                            % makespan: latest completion time

% CID: variável a ser otimizada (sum completion times = sct)
int: sct_min = sum(i in JOB)(d[i,tasks]);  % limite inferior sct
int: sct_max = total * jobs;               % limite superior sct
var sct_min..sct_max: sct;

% CID: variável a ser minimizada
constraint %% calula o sct
    sct = sum(i in JOB) (s[i,tasks]+d[i,tasks]);

% CID: tasks de um mesmo job devem ocorrer em sequência
constraint 
    forall(i in JOB) (
        forall(j in 1..tasks-1) 
            (s[i,j] + d[i,j] <= s[i,j+1]) /\
        s[i,tasks] + d[i,tasks] <= end
    );

% CID: as tarefas não podem se sobrepor no tempo
constraint
    forall(j in TASK) (
        disjunctive([s[i,j] | i in JOB], [d[i,j] | i in JOB])
    );

% CID: otimiza a FO;  estratégia de busca -> prioriza variável 
%      com menor domínio e no seu menor valor.
solve :: int_search(s, first_fail, indomain_min, complete)
      minimize sct;

% CID: teste feito em aula em 9/5/2019, deu um excelente resultado
%solve :: int_search(s, dom_w_deg, indomain_min, complete)
%      minimize sct;

output ["sum completion time = \(sct)\n"] ++
       [ show_int(digs,i) ++ " " ++ 
         show_int(digs,s[i,j]) ++ " " ++ 
         show_int(digs,d[i,j]) ++ " " ++ 
         if j == tasks then "\n" else "" endif |
         i in JOB, j in TASK ];

