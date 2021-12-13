/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include "connection.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {

    if(argc != 3) {
        cout << "Required argument number: 3" << endl;
        return -1;
    }

    Connection con;
    con.connectToServer(argv[1], stoi(argv[2]));
    string message;
    con.receiveFromServer(message, 1000);
    cout << message << endl;
    con.closeConnection();
    return 0;
}
