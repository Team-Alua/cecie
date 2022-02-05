import os
sources_names = []
for fname in os.listdir('source/'):
    sources_names.append('source/' + fname)
    sources_names.append('include/' + fname[:-4] + '.hpp')
' '.join(sources_names)
