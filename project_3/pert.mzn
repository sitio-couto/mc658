% Victor Ferreira Ferrari  - RA 187890
% Vinícius Couto Espindola - RA 188115

% Constraint Programming MiniZinc model for SPLC.
% SPLC: http://www.ic.unicamp.br/~cid/SPLC/SPLC.html

% University of Campinas - 30/05/2019

include "maximum.mzn";
include "exactly.mzn";
include "count_geq.mzn";

%%%% INPUT %%%%
int: L;                             % Available workers
int: no;                            % Amount of orders
array [ORDER] of int: njo;          % Amount of jobs per order
array [ORDER] of int: djo;          % Duration of jobs per order AKA amount of tasks per job.
array [1..tasks] of int: trab;      % Demand of workers for each task.
int: nprec;                         % Amount of precedence relations between jobs of different orders
array [PRECS,1..2] of int: prec;    % Precedence relations between jobs of different orders.

%%%% STRUCTURING INPUT DATA %%%%
% Constants:
int: digs = ceil(log(10.0,end_max));  % format output (amount of digits)
int: jobs = sum(i in ORDER)(njo[i]);  % Amount of jobs
int: tasks = sum(i in ORDER)(djo[i]); % Amount of tasks

% Sets:
set of int: ORDER = 1..no;    % Set of orders indexes
set of int: JOB = 1..jobs;    % Set of jobs indexes
set of int: PRECS = 1..nprec; % Set of different order precedences indexes


%%%% ASSETS %%%%
% JOB ORDER ARRAY: Given a job index, returns its order
array [JOB] of int: j_ord = [o | o in ORDER, n in 1..njo[o]];

% JOB DURATION ARRAY: Given a job index, returns its duration.
array [JOB] of int: djj = [djo[j_ord[j]] | j in JOB];

% BOUNDS:
int: end_min = max(i in ORDER)(djo[i]*njo[i]); % Lower bound: all orders run in parallel. 
int: end_max = sum(i in ORDER)(djo[i]*njo[i]); % Upper bound: all jobs run sequentially.
int: start_max = end_max - min(djo);           % Start time upper bound: last job with all sequential.

% SETS:
set of int: TIME = 0..end_max; % Set of time indexes (each value is a moment)


%%%% VARIABLES %%%%
var end_min..end_max: end;

% Search scope: Start times of each job.
array [JOB] of var 0..start_max: start;

% Table indicating the moment 't' where A job within order 'o' starts
array [ORDER,TIME] of var 0..1: flag;       % Job of order 'o' starts on 't', then flag[o,t] = 1

% Table which calculates workload per order 'o' for each instant 't'.
array [ORDER,TIME] of var 0..L: workload;   % workload[o,t] = <workload of jobs in order o at instant t>


%%%% PRECEDENCE CONSTRAINTS %%%%
% Given the list os precedences (prec) for jobs in different orders, ensures the
% second job of a relation (prec[p,2]) starts after it predecessor (prec[p,1]) ends.
constraint 
    forall(p in PRECS)(
        start[prec[p,2]] >= start[prec[p,1]] + djj[prec[p,1]]
    );

% Ensures that jobs within the same order are scheduled according to its index,
% meaning job i+1 only starts after job i, if both are in the same order.
constraint
    forall(j in 1..(jobs-1) where j_ord[j] == j_ord[j+1])(
        start[j+1] >= start[j] + djj[j]
    );


%%%% STRUCTURING TASK COSTS %%%%%
% First, an array with indexes indicating the index slices of the tasks cost of each
% order within the trab array (slice[i]+1..slice[i+1] = i-th order tasks costs indexes)
array [1..(no+1)] of int: slice = [0,djo[1]]++[sum(k in 1..(i-1))(djo[k])+djo[i] | i in 2..no];

% Second, an array of sets where the i-th array position has the set of indexes which
% point the task costs of the i-th order. Therefore, task_cost[i][k] holds the index 
% of the k-th task from the i-th order, which can be passed to trab, obtaining the cost.
array [ORDER] of set of int: task_costs = [(slice[i]+1)..slice[i+1] | i in ORDER];


%%%% FLAG TABLE CONSTRAINTS %%%%
% Ensures that if a job from order 'o' starts at time 't', then the 
% flag[o,t] shall be set to 1 indicating the jobs schedule
constraint
  forall(j in JOB)(
    flag[j_ord[j],start[j]] = 1
  );

% Ensures that exactly njo[i] (amount jobs of the i-th order) jobs are 
% scheduled (flag set as 1).
constraint
  forall(i in ORDER)(
    exactly(njo[i], flag[i,TIME], 1)
  );

% EXTRA CONSTRAINTS:
% Ensures each starting flag in the i-th order will be spaced by, at least, 
% (djo[i]-1) 0 values. Implying no overlaps between same order jobs.
constraint forall(i in ORDER, j in TIME)(
    if j <= (end_max+1)-djo[i] then 
      count_geq(flag[i,j..(j+djo[i]-1)], 1, 1)
    endif
  );


%%%% WORKLOAD TABLE CONSTRAINTS %%%%
% Ensures the assignment of the tasks costs to the workload table. To do so, it
% uses the flag table to identify where a task of a certain order starts, and 
% then applies the cost profile of that order starting from the instant pointed
% by the flag.
constraint
  forall(o in ORDER, t in TIME)(
    workload[o,t] = sum(k in 1..min(t+1,djo[o]))(flag[o,t+1-k]*trab[task_costs[o][k]])
  );

% Ensures the sum of workforce of every parallel task of diferent order (o)
% does not exceeds at any moment (t) the given limit (L) of workers.
constraint
  forall(t in TIME)(
    sum(workload[ORDER,t]) <= L
  );


%%%% OBJECTIVE CONSTRAINT %%%%
% Attributes to end var the current makespan
constraint maximum(end, [start[i]+djj[i] | i in JOB]);

% SOLVING
solve :: int_search(start, dom_w_deg, indomain_min, complete)
      minimize end;

% OUTPUT:
output [show_int(digs,end) ++ "\n"];
output [show_int(digs,i) ++ " " ++ show_int(digs,start[i]) ++ "\n" | i in JOB];