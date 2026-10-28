#pragma once
#include <dlfcn.h>
#include <cstdint>
#include <unistd.h>

inline uintptr_t GetAddress(const char* libName) {
    void* handle = dlopen(libName, RTLD_LAZY);
    if (!handle) return 0;
    return (uintptr_t)handle;
}
