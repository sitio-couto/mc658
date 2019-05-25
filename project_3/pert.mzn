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

% VARIABLES:
% The objective (last completion time)
var 0..1000: end;

% CONSTRAINTS:

% OUTPUT:
output [show_int(2,end) ++ "\n"];
output [show_int(2,i) ++ "  " ++ "0\n" | i in JOB];