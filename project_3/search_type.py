from sys import argv

code = open("pert.mzn","r")
test = open("testing.mzn","w")

content = code.read()

# Get the position from the openning parenthesis for the
# int_search call
start = content.find("(", content.find("int_search"))+1
end   = content.find(")",start)

if start == -1 or end == -1:
    print("ERROR: Could not find 'int_search' call in pert.mzn")
elif len(argv) < 3:
    print("ERROR: Missing arguments for search_type.py")
else:
    
    for i in [1,2]:
        s = [start-1]
        s += [start+j for j,x in enumerate(content[start:end]) if x == ","]
        s += [end]

        # Change var choice
        if argv[i] != '0':
            content = content[:s[i]+1]+" "+argv[i]+content[s[i+1]:]

test.write(content)
code.close()
test.close()
