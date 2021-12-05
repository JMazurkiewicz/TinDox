#pragma once

#include <ext/stdio_filebuf.h>

namespace tds::linux {
    using StreamBuf = __gnu_cxx::stdio_filebuf<char>;
}
