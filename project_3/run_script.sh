
python search_type.py smallest indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_min" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py smallest indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_median" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py smallest indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_random" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py smallest indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_split" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py first_fail indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_min" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py first_fail indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_median" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py first_fail indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_random" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py first_fail indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_split" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_min" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_median" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_random" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_split" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py most_constrained indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_min" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py most_constrained indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_median" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py most_constrained indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_random" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py most_constrained indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_split" >> results.txt
echo "INSTANCE: Ins_3o_7j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_3o_7j_A.dzn >> results.txt
python search_type.py smallest indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py smallest indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py smallest indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py smallest indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py first_fail indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py first_fail indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py first_fail indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py first_fail indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py most_constrained indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py most_constrained indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py most_constrained indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py most_constrained indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_21j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_21j_A.dzn >> results.txt
python search_type.py smallest indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py smallest indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py smallest indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py smallest indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py first_fail indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py first_fail indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py first_fail indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py first_fail indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py most_constrained indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py most_constrained indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py most_constrained indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py most_constrained indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_23j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_23j_A.dzn >> results.txt
python search_type.py smallest indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py smallest indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py smallest indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py smallest indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py first_fail indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py first_fail indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py first_fail indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py first_fail indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py most_constrained indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py most_constrained indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py most_constrained indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py most_constrained indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_A.dzn >> results.txt
python search_type.py smallest indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py smallest indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py smallest indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py smallest indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py first_fail indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py first_fail indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py first_fail indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py first_fail indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py dom_w_deg indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py dom_w_deg indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py dom_w_deg indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py dom_w_deg indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py most_constrained indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py most_constrained indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py most_constrained indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py most_constrained indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_24j_B.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_24j_B.dzn >> results.txt
python search_type.py smallest indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py smallest indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py smallest indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py smallest indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: smallest indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py first_fail indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py first_fail indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py first_fail indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py first_fail indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: first_fail indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py dom_w_deg indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: dom_w_deg indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py most_constrained indomain_min
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_min" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py most_constrained indomain_median
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_median" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py most_constrained indomain_random
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_random" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
python search_type.py most_constrained indomain_split
echo "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%" >> results.txt
echo "PARAMS: most_constrained indomain_split" >> results.txt
echo "INSTANCE: Ins_4o_27j_A.dzn" >> results.txt
minizinc testing.mzn -s -t 10000 Instances/Ins_4o_27j_A.dzn >> results.txt
