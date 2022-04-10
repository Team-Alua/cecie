#pragma once

#include <functional>

void folderWalker(const char * root, const char * relativePath,  std::function<void(const char*, const char *)>);

