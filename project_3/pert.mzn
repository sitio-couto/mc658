% Victor Ferreira Ferrari  - RA 187890
% Vinícius Couto Espindola - RA 188115

% Constraint Programming MiniZinc model for SPLC.
% SPLC: http://www.ic.unicamp.br/~cid/SPLC/SPLC.html

% University of Campinas - 30/05/2019

% INPUT:
int: L;                             % Available workers
int: no;                            % Amount of orders
set of int: ORDER = 1..no;
array [ORDER] of int: njo;          % Amount of jobs per order
                                    % Jobs from 1 to njo[1] are in order 1, and so on.
array [ORDER] of int: djo;          % Duration of jobs per order.
int: jobs = sum(i in ORDER)(djo[i]);
set of int: JOB = 1..jobs;
array [JOB] of int: trab;           % Demand of workers for each jobs.
                                    % The djo[1] first indices describe the worker profile for order 1,
                                    % the djo[2] following indices for order 2, and so on.
int: nprec;                         % Amount of precedence relations between jobs of different orders
array [1..nprec,1..2] of int: prec; % Precedence relations between jobs of different orders.

% BOUNDS:
% Lower bound for end time: all orders run in parallel. No worker or precedence restrictions. 
% Max order duration.
int: end_min = max(i in ORDER)(djo[i]*njo[i]);

% Upper bound for end time: all jobs run sequentially.
int: end_max = sum(i in ORDER)(djo[i]*njo[i]);

% Upper bound for start time: last job with all sequential.
int: start_max = end_max - min(djo);

int: digs = ceil(log(10.0,end_max));

% VARIABLES:
% Start time of each job.
array [JOB] of var 0..start_max: start;

% The objective (last completion time)
var end_min..end_max: end;

% CONSTRAINTS:

% Solving
solve :: int_search(start, first_fail, indomain_min, complete)
      minimize end;

% OUTPUT:
output [show_int(digs,end) ++ "\n"];
output [show_int(digs,i) ++ " " ++ show_int(digs,start[i]) ++ "\n" | i in JOB];