#pragma once

#include <functional>

#include "_types/filesystem.hpp"

void folderWalker(const char * root, const char * relativePath,  std::function<void(const char*, const char *)>);

