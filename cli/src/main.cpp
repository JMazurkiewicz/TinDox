/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include <csignal>
#include "tui.h"

int main() {
    signal(SIGPIPE, SIG_IGN);

    try {
        Tui tui("127.0.0.1", 37666);
        tui.runTDPClient();
    } catch (const return_exception &ex) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}