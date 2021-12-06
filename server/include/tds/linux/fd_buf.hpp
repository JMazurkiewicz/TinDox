#pragma once

#include <ext/stdio_filebuf.h>

namespace tds::linux {
    using FdBuf = __gnu_cxx::stdio_filebuf<char>;
}
