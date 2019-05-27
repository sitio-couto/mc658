from sys import argv

code = open("pert.mzn","r")
test = open("testing.mzn","w")

content = code.read()

# Get the position from the openning parenthesis for the
# int_search call
start = content.find("(", content.find("int_search"))+1
end   = content.find(")",start)
content = list(content)

if start == -1 or end == -1:
    print("ERROR: Could not find 'int_search' call in pert.mzn")
elif len(argv) < 3:
    print("ERROR: Missing arguments for search_type.py")
else:
    # print(content[start:end])
    params = [start-1]
    params += [start+i for i,x in enumerate(content[start:end]) if x == ","]
    params += [end]
    
    # Change var choice
    if argv[1] != '0':
        content[params[1]+1:params[2]] = argv[1]
    # Change constraint choice
    if argv[2] != '0':
        content[params[2]+1:params[3]] = argv[2]
    

test.write("".join(content))
code.close()
test.close()
