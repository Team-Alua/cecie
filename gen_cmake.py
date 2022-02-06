#!/usr/bin/python3 
import ast
import sys
tmpl = sys.argv[1]
output = ""
def getCodeLine(statement,code):
    lines = code.split("\n")
    startLine = statement.lineno - 1
    endLine = statement.end_lineno - 1
    colStart = statement.col_offset
    colEnd = statement.end_col_offset
                      
    if startLine == endLine:
        return lines[startLine][colStart:colEnd]
    codeLines = ""
    codeLines += lines[startLine][colStart:] + "\n"
    for line in lines[startLine + 1:endLine]:
        codeLines += line + "\n"
    codeLines += lines[endLine][:colEnd] + "\n"
    return codeLines

def dofile(name):
    searchPaths = [
        '',
        'scripts/'
    ]
    for searchPath in searchPaths:
        try:
            
            with open('{}{}.py'.format(searchPath, name), 'r') as execFile:
                return execCode(execFile.read())
        except FileNotFoundError:
            pass
    raise Exception('Could not find {}'.format(name))

def execCode(code, session_locals = locals()):
    code = code.strip()
    predef_globals = globals()
    session_locals = session_locals.update({
        'dofile': dofile,
    })
    code_ast = ast.parse(code)
    nodes = ast.iter_child_nodes(code_ast)
    statements = list(nodes)
    if len(statements) == 0:
        return None
    if not isinstance(statements[-1], ast.Expr):
        raise TypeError("Expected statement but got:\n{}".format(getCodeLine(statements[-1], code)))

    if len(statements) > 1:
        compiled_statements = compile(ast.Module(body=statements[:-1], type_ignores=[]), "<ast>", "exec")
        exec(compiled_statements, predef_globals, session_locals)
    compiled_line = compile(ast.Expression(statements[-1].value), "<ast>", "eval")
    return eval(compiled_line, predef_globals, session_locals)

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
        generated += execCode(code)        
        line = line[endIndex + len(codeEnd):]
    return generated
    
with open(tmpl, 'r') as tmplFile:
    for line in tmplFile:
       output += resolveLine(line) + "\n" 

with open('CMakeLists.txt', 'w') as outFile:
    outFile.write(output)


