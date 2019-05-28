from sys import argv

# PASS THE FOLLOWING PARAMETERS:
# <time_limit>  = given in seconds
# <output_file> = name of file to print results
# <type_gen>    = how to combine varchoice and contraintchoice
#  type_gen = 1 => only change varchoice in the given scope 
#  type_gen = 2 => only change contraintchoice in the given scope 
#  type_gen = 3 => combine var and contraintchoice in their given scopes 

# Select how many elements from the list od all possible choices 
# will be chosen to iterate.
VS = 4
CS = 4

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

# Get output file to be create with results
if len(argv) >= 4:
    time_limit = int(argv[1])*1000
    out_file = argv[2]
else:
    print("MISSING ARGUMENTS: <time(s)> <output_file> <type_gen>")
    exit(1)

splitter = "\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%\%"
file_name = "testing.mzn"
script_name = "run_script.sh"
command = "minizinc "+file_name+" -s -t "+str(time_limit)

vcs = []
ccs = []
for vc in varchoice[:VS]:
    vcs.append(vc)
for cc in constchoice[:CS]:
    ccs.append(cc)

output = []
# Generate only chaging varchoices
if (argv[3]=="1"):
    for i in small:
        for vc in vcs:
            output.append("\npython search_type.py "+vc+" "+cc)
            output.append('\necho "'+splitter+'"'+" >> "+out_file)
            output.append('\necho "' +"PARAMS: "+vc+" "+"0"+'" >> '+out_file)
            output.append('\necho "'+'INSTANCE: '+i+'" >> '+out_file)
            output.append("\n"+command+" Instances/"+i+" >> "+out_file)
# Generate only changing constraintchoices
elif (argv[3]=="2"):
    for i in small:
        for cc in ccs:
            output.append("\npython search_type.py "+vc+" "+cc)
            output.append('\necho "'+splitter+'"'+" >> "+out_file)
            output.append('\necho "' +"PARAMS: "+"0"+" "+cc+'" >> '+out_file)
            output.append('\necho "'+'INSTANCE: '+i+'" >> '+out_file)
            output.append("\n"+command+" Instances/"+i+" >> "+out_file)
# Generate combining both var and contraint choices
elif (argv[3]=="3"):
    for i in small:
        for vc in vcs:
            for cc in ccs:
                output.append("\npython search_type.py "+vc+" "+cc)
                output.append('\necho "'+splitter+'"'+" >> "+out_file)
                output.append('\necho "' +"PARAMS: "+vc+" "+cc+'" >> '+out_file)
                output.append('\necho "'+'INSTANCE: '+i+'" >> '+out_file)
                output.append("\n"+command+" Instances/"+i+" >> "+out_file)
    

script = open(script_name,"w")
map(script.write,output)
script.write("\n")
script.close()
