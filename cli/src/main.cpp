/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include <csignal>
#include "tui.h"

int main() {
    signal(SIGPIPE, SIG_IGN);

    Tui tui("127.0.0.1", 37666);
    tui.runTDPClient();

//    TDPService tdp;
//    tdp.initConnection("127.0.0.1", 37666);
//    tdp.auth("admin", "admin");
//    tdp.cd("dir spc");
//    tdp.pwd();
//    string res = tdp.response_body;

    return 0;
}