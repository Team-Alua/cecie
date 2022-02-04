#!/usr/bin/python3 

import sys
tmpl = sys.argv[1]
output = ""
def execCode(code):
    tmpLocal = {}
    exec(code, globals(), tmpLocal)
    return tmpLocal.get("result", "")

def resolveLine(line):
    originalLine = line
    generated = ""
    codeStart = "<py:"
    codeEnd = ":py>"
    while True:
        startIndex = line.find(codeStart)
        if startIndex == -1:
            generated += line
            break
        generated += line[:startIndex]

        startIndex += len(codeStart)
        endIndex = line.find(codeEnd, startIndex)
        if endIndex == -1:
            generated = originalLine
            break
        code = line[startIndex: endIndex]
        print(code)
        generated += execCode(code)        
        line = line[endIndex + len(codeEnd):]
    return generated
    
with open(tmpl, 'r') as tmplFile:
    for line in tmplFile:
       output += resolveLine(line) + "\n" 

with open('CMakeLists.txt', 'w') as outFile:
    outFile.write(output)


