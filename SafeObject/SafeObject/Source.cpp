#include "SafeObject.h"

#include <cstdio>
#include <utility>

int main(int argc, char *argv[])
{
    AutoClean::SafeHandle handle1 = CreateFile(L"testfile1.txt", GENERIC_READ, 0, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    AutoClean::SafeHandle handle2 = CreateFile(L"testfile2.txt", GENERIC_READ, 0, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    fprintf(stderr, "Handle 1: %X\n"
        "Handle 2: %X\n",
        handle1.Get(), handle2.Get());

    handle1 = std::move(handle2);

    fprintf(stderr, "Handle 1: %X\n"
        "Handle 2: %X\n",
        handle1.Get(), handle2.Get());

    return 0;
}