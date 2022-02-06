#!/usr/bin/python3
import os
import sys
def insert_into_section(lines, section_name, section_lines):
    segmentStart = -1
    segmentEnd = len(lines)
    for index, line in enumerate(lines):
        if line.strip() == "//seg:" + section_name:
            segmentStart = index + 1
            break
    for index, line in enumerate(lines[segmentStart:]):
        if line.strip().startswith("//seg:"):
            segmentEnd = index + segmentStart 
            break
    if segmentStart == -1:
        raise Exception("{} was not found".format(section_name))
    new_lines = lines[:segmentStart]
    new_lines += section_lines
    new_lines += lines[segmentEnd:]
    return new_lines
    
def cmdNameToTitleCase(cmd_name):
    return ''.join([piece.title() for piece in cmd_name.split('_')])

cmd_name = (sys.argv[1] if len(sys.argv) > 1 else input('Enter command name:')).strip()
file_format = r"""
// Auto generated
#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"

bool {}(int connfd, cmd_args & args) {{
{tab}log("{} was called!"); 
}}
""".lstrip()
with open('source/_cmds/{}.cpp'.format(cmd_name), 'w') as cmdFile:
    code = file_format.format(cmd_name, cmdNameToTitleCase(cmd_name),tab="\t")
    cmdFile.write(code)

cmds = os.listdir('source/_cmds/')
lines = [
    "#pragma once",
    "#include \"cmd_common.hpp\"",
    "",
]
cmds_list = [cmd.replace(".cpp", "") for cmd in cmds]

lines += ["bool {}(int connfd, cmd_args & args);".format(cmd) for cmd in cmds_list]
lines += [''];

cmd_list = '\n'.join(lines)
with open('include/cmd_list.hpp', 'w') as cmdListFile:
    cmdListFile.write(cmd_list)

code_lines = []
code_lines += [
    "",
    "void cmd_init(CommandHandler & handler) {"
]
code_lines += ["\thandler.add(\"{}\", {});".format(cmdNameToTitleCase(cmd), cmd) for cmd in cmds_list]
code_lines += ['}', '']
handler_lines = []
with open('source/cmd_handler.cpp', 'r') as cmdHandlerFile:
    handler_lines = cmdHandlerFile.read().split('\n')

handler_lines = insert_into_section(handler_lines, 'cmd_init', code_lines)
handler_code = '\n'.join(handler_lines)
print(handler_code)
with open('source/cmd_handler.cpp', 'w') as cmdListSrcFile:
    cmdListSrcFile.write(handler_code)

