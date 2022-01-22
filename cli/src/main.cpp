/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include "tdpservice.h"
#include <iostream>
#include <csignal>

using namespace std;

int main() {
    signal(SIGPIPE, SIG_IGN);

    TDPService service;
    service.initConnection("127.0.0.1", 37666);

    service.auth("admin", "admin");
    cout << service.last_sent_command << service.received_response;

    service.cd("/");
    cout << service.last_sent_command << service.received_response;

    service.name();
    cout << service.last_sent_command << service.received_response;

    service.perms();
    cout << service.last_sent_command << service.received_response;

    service.rename("big", "very_big");
    cout << service.last_sent_command << service.received_response;

    service.pwd();
    cout << service.last_sent_command << service.received_response;

    service.mkdir("Lukasz");
    cout << service.last_sent_command << service.received_response;

    service.cp("Makefile", "/Lukasz");
    cout << service.last_sent_command << service.received_response;

    service.mv("Makefile", "/");
    cout << service.last_sent_command << service.received_response;

    service.ls("", "true", "true");
    cout << service.last_sent_command << service.received_response;

    service.rm("Lukasz");
    cout << service.last_sent_command << service.received_response;

    service.tree("");
    cout << service.last_sent_command << service.received_response;

    service.logout();
    cout << service.last_sent_command << service.received_response;

    service.closeConnection();
    return 0;
}
