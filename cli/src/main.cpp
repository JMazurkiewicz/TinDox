/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include "tdpservice.h"
#include <iostream>
#include <sstream>

using namespace std;

int main() {

    TDPService service;
    service.initConnection("127.0.0.1", 37666);
    service.auth("admin", "admin");
    service.closeConnection();
    return 0;
}
