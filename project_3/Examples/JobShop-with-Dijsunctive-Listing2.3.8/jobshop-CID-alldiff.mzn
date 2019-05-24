

include "disjunctive.mzn";
include "alldifferent.mzn";

int: jobs;                                    % no of jobs
set of int: JOB = 1..jobs;
int: tasks;                                   % no of tasks per job
set of int: TASK = 1..tasks;
array [JOB,TASK] of int: d;                   % task durations
int: total = sum(i in JOB, j in TASK)(d[i,j]);% total duration
int: digs = ceil(log(10.0,total));            % digits for output
array [JOB,TASK] of var 0..total: s;          % start times
var 0..total: end;                            % total end time

% CID: to use the alldifferent global constraint. THere is an optimal
%      solution which is a permutation schedule. Let's use this.
array[1..jobs] of var 1..jobs: perm;  % perm[i]: i-th element of the permutation

% CID: variável a ser otimizada (average completion time)
int: sct_min = sum(i in JOB)(d[i,tasks]);  % limite inferior sct
int: sct_max = total * jobs;               % limite superior sct
var sct_min..sct_max: sct;

% CID: 
constraint %% calula o sct
    sct = sum(i in JOB) (s[i,tasks]+d[i,tasks]);

% CID: 
constraint %% permutation contains different values only
    alldifferent(perm);

% Original ...
constraint %% ensure the tasks occur in sequence
    forall(i in JOB) (
        forall(j in 1..tasks-1) 
            (s[i,j] + d[i,j] <= s[i,j+1]) /\
        s[i,tasks] + d[i,tasks] <= end
    );

constraint %% ensure no overlap of tasks
    forall(j in TASK) (
        disjunctive([s[i,j] | i in JOB], [d[i,j] | i in JOB])
    );

% CID: redundant, but uses the fact that the solution is a permutation schedule
constraint %% the starting time of the first job of the permutation in machine 1 is NULL
    s[perm[1],1]=0;

constraint %% the startin time of the first job of the permutation in machine 2 is known
    s[perm[1],2]=s[perm[1],1]+d[perm[1],1];

constraint %% only for machine 1: no idle time between tasks, from the 2nd job of perm on
    forall(i in 2..jobs) (
        s[perm[i],1] = s[perm[i-1],1]+d[perm[i-1],1]
    );

constraint %% all other machines, from the second job of the permutation on
    forall(i in 2..jobs, j in 2..tasks) (
        s[perm[i],j] >= s[perm[i-1],j]+d[perm[i-1],j]
    );

% FO original: solve minimize end;
% CID: nova FO
solve :: int_search(perm, first_fail, indomain_min, complete)
      minimize sct;

output ["sum completion time = \(sct)\n"];

output ["permutation: \n"] ++  [ show_int(digs,perm[i]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

% CID: starting times of jobs in machine 1
output ["starting times of jobs in machine 1\n"];
output [ "(" ++ show_int(digs,i) ++ ")" ++  show_int(digs,s[i,1]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

% CID: starting times of jobs in machine 2
output ["starting times of jobs in machine 2\n"];
output [ "(" ++ show_int(digs,i) ++ ")" ++  show_int(digs,s[i,2]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];

% CID: starting times of jobs in machine 2
output ["completion times of jobs in machine 2\n"];
output [ "(" ++ show_int(digs,i) ++ ")" ++  show_int(digs,s[i,2]+d[i,2]) ++ " " ++
         if i == jobs then "\n" else "" endif |
         i in JOB];


