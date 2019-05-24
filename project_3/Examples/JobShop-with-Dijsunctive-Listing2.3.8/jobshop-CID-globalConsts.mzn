%---------------------------------------------------------------------
% CID: solve the flowshop problem with (2 machines only)
%      using the inverse global constraint (2019/03).
%---------------------------------------------------------------------

include "inverse.mzn";

int: jobs;                                    % no of jobs
set of int: JOB = 1..jobs;
int: tasks;                                   % no of tasks per job
set of int: TASK = 1..2;
array [JOB,TASK] of int: d;                   % task durations
int: total = sum(i in JOB, j in TASK)(d[i,j]);% total duration
int: digs = ceil(log(10.0,total));            % digits for output

% CID: Two  views of the  problem. The  solution is a  permutation. We
% also keep the position of the element in the permutation so that 
% perm[i]=j <-> pos[j]=i
% This way we could use the inverse global constraint.
array[0..jobs] of var 0..jobs: perm;  % perm[i]: i-th element of the permutation
array[0..jobs] of var 0..jobs: pos;   % pos[j]: position of element j in the permutation

% CID: bounds for f1 and f2 values
int: f1_max = sum(i in JOB)(d[i,1]);       % upper bound for f1
int: d1_max = max(i in JOB)(d[i,1]);       % max processing time in machine 1  
int: d2_max = max(i in JOB)(d[i,2]);       % max processing time in machine 2
int: d1_min = min(i in JOB)(d[i,1]);       % min processing time in machine 1     

% Upper bound for the finishing time of a JOB in machine 2. 
% If d1_max > d2_max: assume all jOBS in machine 1 take d1_max and all
% JOBS in machine 2 run in parallel with a JOB in machine 1, but the last one.
% In this case, the UB=jobs*d1_max+d2_max
% If d2_max > d1_max: the starting time of all JOBS in machine 2 (except the 1st one) 
% are constrained by the finishing time of the JOB that precedes it in machine 2. In
% other words, all JOBS in machine 1 run in parallel with a JOB in machine 2, but the 
% first one. So,  in this case, the UB=jobs*d2_max+d1_max
int: f2_max = max(jobs*d1_max+d2_max,jobs*d2_max+d1_max);  
% min time for the first job to finish in machine 2 
int: f2_min = min(i in JOB)(d[i,1]+d[i,2]);  

% CID: completion times in the two machines
array [JOB] of var d1_min..f1_max: f1;          % completion time machine 1
array [0..jobs] of var 0..f2_max: f2;      % completion time machine 2

% CID: the objective (sum of  completion times)
int: sct_min = sum(i in JOB)(d[i,tasks])+min(i in JOB)(d[i,1]);  % lower bound for sct
int: sct_max = f2_max*jobs-sum(i in JOB)(d[i,2])+d2_max;         % limite superior sct
var sct_min..sct_max: sct;
  
% CID: calula a soma dos completion times
constraint 
    sct = sum(i in JOB) (f2[i]);

% CID: constraint on  f1 de todos os jobs
constraint %% no idle time in machine 1
    forall(i in JOB) (
        f1[i]=sum(j in JOB where pos[j] <= pos[i])(d[j,1])
    );

% CID: dummy job ZER0 completed at time d[pos[1],1] in machine 2
%      (to help describing contraint[*])
constraint f2[0]=0;

% CID: dummy job ZER0 always the 0-th one in the permutation
%      (to help describing contraint[*])
constraint perm[0]=0;

% CID: dummy job ZER0 always in the 0-th position
%      (to help describing contraint[*])
constraint pos[0]=0;

% CID: lower bound on the finishing times in machine 2
constraint forall(i in JOB) ( f2[i] >= f2_min );

% CID: job i can only start in machine 2 after it has finished 
%      in machine 1
constraint forall(i in JOB) ( f2[i] - d[i,2] >= f1[i] );

% CID: job in machine 2 can only start if previous job was completed in machine 2
constraint forall(i in JOB) ( f2[i] - d[i,2] >= f2[perm[pos[i]-1]] );

% CID: inverse (see manual: does the link between perm and pos);  chanelling
constraint inverse(perm,pos);

% CID:  FO
%solve :: seq_search([
%int_search(perm, first_fail, indomain_min, complete),
%int_search([sct], smallest, indomain_min, complete)])
solve :: int_search(perm, first_fail, indomain_min, complete)
      minimize sct;

output ["sum completion time = \(sct)\n"];

output ["permutation: \n"] ++  [ show_int(digs,perm[i]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

output ["position: \n"] ++  [ show_int(digs,pos[i]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

% CID: starting times of jobs in machine 1
output ["starting times of jobs in machine 1\n"];
output [ "(" ++ show_int(digs,i) ++ ")" ++  show_int(digs,(f1[i]-d[i,1])) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

% CID: starting times of jobs in machine 2
output ["starting times of jobs in machine 2\n"];
output [ "(" ++ show_int(digs,i) ++ ")" ++  show_int(digs,(f2[i]-d[i,2])) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

% CID: starting times of jobs in machine 2
output ["completion times of jobs in machine 2\n"];
output [ "(" ++ show_int(digs,i) ++ ")" ++  show_int(digs,f2[i]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

%solve :: int_search(perm, first_fail, indomain_random, complete)
%      :: restart_geometric(2.0,jobs)
%      :: restart_linear(1000)


