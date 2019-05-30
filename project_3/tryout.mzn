% Use this editor as a MiniZinc scratch book
include "globals.mzn";

array [1..3,1..1] of int: a = [|1|2|3|];
set of int: DOM = 1..3;

array [1..3] of var DOM: x;

constraint 
  forall(i in DOM)(
    table([x[i]], a)
  );

constraint increasing(x);
constraint alldifferent(x);

solve satisfy;

output ["Values: \(x)\n"];