import os
from pathlib import Path

sources_names = []
for fname in list(Path("source/").rglob("*.cpp")):
    oFileName = '${EXEC_DIR}' + str(fname) + '.o'
    sources_names.append(oFileName)
' '.join(sources_names)

