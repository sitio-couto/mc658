% Victor Ferreira Ferrari  - RA 187890
% Vinícius Couto Espindola - RA 188115

% Constraint Programming MiniZinc model for SPLC.
% SPLC: http://www.ic.unicamp.br/~cid/SPLC/SPLC.html

% University of Campinas - 30/05/2019

%include "globals.mzn";
include "arg_max.mzn";

% INPUT:
int: L;                             % Available workers
int: no;                            % Amount of orders
set of int: ORDER = 1..no;
array [ORDER] of int: njo;          % Amount of jobs per order
                                    % Jobs from 1 to njo[1] are in order 1, and so on.
int: jobs = sum(i in ORDER)(njo[i]);
set of int: JOB = 1..jobs;
array [ORDER] of int: djo;          % Duration of jobs per order AKA amount of tasks per job.
int: tasks = sum(i in ORDER)(djo[i]);
array [1..tasks] of int: trab;      % Demand of workers for each task.
                                    % The djo[1] first indices describe the worker profile for order 1,
                                    % the djo[2] following indices for order 2, and so on.
int: nprec;                         % Amount of precedence relations between jobs of different orders
set of int: PRECS = 1..nprec;
array [1..nprec,1..2] of int: prec; % Precedence relations between jobs of different orders.

% BOUNDS:
% Lower bound for end time: all orders run in parallel. No worker or precedence restrictions. 
% Max order duration.
int: end_min = max(i in ORDER)(djo[i]*njo[i]);

% Upper bound for end time: all jobs run sequentially.
int: end_max = sum(i in ORDER)(djo[i]*njo[i]);
set of int: TIME = 0..end_max;

% Upper bound for start time: last job with all sequential.
int: start_max = end_max - min(djo);


% CONSTANTS:
% Order of each job.
array [JOB] of var ORDER: j_order;
constraint forall(i in ORDER, j in 1..njo[i])(j_order[sum(k in 1..i-1)(njo[k])+j] = i);

% MISC:
int: digs = ceil(log(10.0,end_max));


% VARIABLES:
% Start time of each job.
array [JOB] of var 0..start_max: start;

% Workers per time unit (optimize if possible).
array [TIME] of var 0..L: workers;

% The objective (last completion time)
var end_min..end_max: end;

% CONSTRAINTS:
% End time (better than maintaining end times?)
% Objective function.
constraint end = max(start) + djo[j_order[arg_max(start)]];

% Precedence: second has to start after first ends.
constraint 
    forall(i in PRECS)(
        start[prec[i,2]] >= start[prec[i,1]] + djo[j_order[prec[i,1]]]
    );

% Order: job i+1 only starts after job i, if both are in the same order.
constraint
    forall(i in 1..jobs-1 where j_order[i] == j_order[i+1])(
        start[i+1] >= start[i] + djo[j_order[i]]
    );

% TODO: worker constraints
% Todo instante de tempo tem um numero de trabalhadores trabalhando
% O numero de trabalhadores em um instante é a soma de todos os tasks que estão rodando naquela hora
% não tem preempção, então se um job começar, tem que terminar
% ainda assim, pode começar um job no meio de outro, se der para os trabalhadores conviverem juntos.
% TLDR: não dá para associar trabalhadores a jobs, tem que considerar cada task dentro deles.
% Mas não pode considerar demais, pq não pode parar um job no meio.
% Não precisa usar L, pq já está no bound da variável.

% When a job starts, the first task starts (wrong: accounts for only one job starting).
%constraint forall(i in JOB)(workers[start[i]] = trab[sum(k in 1..j_order[i])(djo[k])]);

% BAITS COMUNS:
% Cumulative: como os trabalhadores são por task, não dá para fazer isso com jobs (sem criatividade)
% Tempo gasto nisso >1h
% Dual Modelling: Não dá para fazer "qual job começou nesse instante" pq pode ter job em paralelo 
% Tempo gasto nisso =~20min

%%%% TESTS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
array [ORDER,TIME] of var {0,1}: flags;
array [ORDER,TIME] of var int: workload;

array [1..(no+1)] of int: split = [0,djo[1]]++[sum(k in 1..(i-1))(djo[k])+djo[i] | i in 2..no];
array [ORDER] of set of int: task_costs = [(split[i]+1)..split[i+1] | i in ORDER];

constraint 
  forall(j in JOB)(
    flags[j_order[j],start[j]] = 1
  );
  
constraint
  forall(i in ORDER,j in TIME)(
    if j >= djo[i]-1 then 
      workload[i,j] = sum(k in 1..djo[i])(flags[i,j+1-k]*trab[task_costs[i][k]])
    else
      workload[i,j] = sum(k in 1..(j+1))(flags[i,j+1-k]*trab[task_costs[i][k]])
    endif
  );

output ["TEST:\n\(trab)\n\(task_costs[1][1])\n"];
output ["start times:\n\(start)\n"];
output ["testing flags table:\n\(flags[1,0..end_max])\n\(flags[2,TIME])\n\(flags[3,TIME])\n"];
output ["testing costs table:\n\(workload[1,TIME])\n\(workload[2,TIME])\n\(workload[3,TIME])\n"];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Solving
solve :: int_search(start, first_fail, indomain_min, complete)
      minimize end;

% OUTPUT:
output [show_int(digs,end) ++ "\n"];
output [show_int(digs,i) ++ " " ++ show_int(digs,start[i]) ++ "\n" | i in JOB];