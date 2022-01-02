/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include "tdpservice.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {

    if (argc != 3) {
        cout << "Required argument number: 3" << endl;
        return -1;
    }

    TDPService service;
    service.initConnection(argv[1], stoi(argv[2]));
    cout << service.response;
    service.closeConnection();
    return 0;
}
