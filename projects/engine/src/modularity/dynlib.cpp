#include "dynlib.hpp"

#ifdef __linux
#include <dlfcn.h>
#endif

#include <iostream>
#include <stdexcept>

namespace Manta {
    DynLib *DynLib::Open(const std::string &path) {
        DynLib* lib = new DynLib();

#ifdef __linux__
        lib->handle = dlopen(path.c_str(), RTLD_LAZY);
#endif

        return lib;
    }

    void *DynLib::GetFunction(const std::string &signature) {
        if (!IsValid()) {
#if __linux__
            std::cerr << dlerror() << std::endl;
#endif

            throw std::logic_error("DynLib handle was invalid!");
        }

#ifdef __linux__
        return dlsym(handle, signature.c_str());
#endif
    }

    bool DynLib::IsValid() {
#ifdef __linux__
        return handle != nullptr;
#endif
    }
}