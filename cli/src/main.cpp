/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include "connection.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {

    try {
        if (argc != 3) {
            cout << "Required argument number: 3" << endl;
            return -1;
        }

        Connection con;
        con.connectToServer(argv[1], stoi(argv[2]));
        string message;
        con.receiveAllReadyFromServer(message);
        cout << message << endl;
        con.closeConnection();
    } catch (const std::system_error& ex) {
        std::stringstream error_message("");
        error_message << ex.code() << '\n' << ex.what() << '\n';
        perror(error_message.str().c_str());
    }
    return 0;
}
