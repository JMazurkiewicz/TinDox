#pragma once

#include <ext/stdio_filebuf.h>

namespace tds::linux {
    using Streambuf = __gnu_cxx::stdio_filebuf<char>;
}
