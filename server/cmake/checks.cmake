include(CheckSymbolExists)

check_symbol_exists(epoll_create1 "sys/epoll.h" TDS_EPOLL_EXISTS)
check_symbol_exists(pipe2 "unistd.h" TDS_PIPE2_EXISTS)
check_symbol_exists(tcgetattr "termios.h" TDS_TCGETATTR_EXISTS)
check_symbol_exists(tcsetattr "termios.h" TDS_TCSETATTR_EXISTS)
