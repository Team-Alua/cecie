import os
from pathlib import Path
sources_names = []
for fname in list(Path("source/").rglob("*.cpp")):
    sources_names.append(str(fname))

' '.join(sources_names)

