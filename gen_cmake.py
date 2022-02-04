#!/usr/bin/python3 

import sys
tmpl = sys.argv[1]
output = ""
with open(tmpl, 'r') as tmplFile:
    for line in tmplFile:
        if line.strip().startswith("#python:"):
            line = line.strip()
            codeToExec = line[len("#python:"):]
            codeToExec = codeToExec.strip()
            tmpLocal = {}
            exec(codeToExec, globals(), tmpLocal)
            output += tmpLocal.get("result", "") + "\n"
        else:
            output += line + "\n"

with open('CMakeLists.txt', 'w') as outFile:
    outFile.write(output)


