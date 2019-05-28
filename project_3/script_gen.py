

# Lists with all possible values for var and contraint choice
varchoice = ["smallest","first_fail","dom_w_deg","most_constrained",
             "input_order","occurrence","anti_first_fail","impact",
             "largest","max_regret"]
constchoice = ["indomain_min","indomain_median","indomain_random",
               "indomain_split","indomain","indomain_interval",
               "indomain_max","indomain_middle","indomain_reverse_split",
               "indomain_split_random","outdomain_max","outdomain_median",
               "outdomain_min","outdomain_random"]
# All instances files

small = ["Ins_3o_7j_A.dzn", "Ins_4o_21j_A.dzn", "Ins_4o_23j_A.dzn", "Ins_4o_24j_A.dzn",
         "Ins_4o_24j_B.dzn", "Ins_4o_27j_A.dzn"]

medium = ["Ins_6o_41j_A.dzn", "Ins_6o_41j_B.dzn", "Ins_6o_41j_C.dzn", "Ins_6o_44j_A.dzn", "Ins_6o_44j_B.dzn",
          "Ins_8o_63j_A.dzn", "Ins_8o_63j_B.dzn", "Ins_8o_63j_C.dzn", "Ins_8o_65j_A.dzn", "Ins_8o_65j_B.dzn"]

large = ["Ins_10o_100j_A.dzn", "Ins_10o_102j_A.dzn", "Ins_10o_106j_A.dzn", "Ins_10o_84j_A.dzn",
         "Ins_10o_84j_B.dzn", "Ins_10o_84j_B.dzn", "Ins_10o_87j_A.dzn", "Ins_10o_88j_A.dzn",
         "Ins_12o_108j_A.dzn", "Ins_12o_109j_A.dzn"]

splitter = "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%"
time_limit = 180*1000
file_name = "testing.mzn"
out_file = "results.txt"
script_name = "run_script.sh"
command = "minzinc "+file_name+" -s -t "+str(time_limit)

params = []
for vc in varchoice[:4]:
    for cc in constchoice[:4]:
        params.append((vc,cc))

output = []
for i in small:
    for (vc,cc) in params:
        output.append("search_type.py "+vc+" "+cc)
        output.append('echo "'+splitter+'"'+" >> "+out_file)
        output.append('echo "' +"PARAMS: "+vc+" "+cc+'" >> '+out_file)
        output.append('echo "'+'INSTANCE: '+i+'" >> '+out_file)
        output.append(command+" Instance "+i+" >> "+out_file)

script = open(script_name,"w")
for i in output:
    print(i)