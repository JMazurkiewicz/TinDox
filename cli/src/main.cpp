/* TIN21Z - Klient konsolowy
 * Autor: Łukasz Reszka (300257)
 * */

#include "tdpservice.h"
#include <iostream>

using namespace std;

int main() {

	TDPService service;
	service.initConnection("127.0.0.1", 37666);

	service.auth("admin", "admin");
	cout << service.last_sent_command << service.received_response;

	service.rename("big", "very_big");
	cout << service.last_sent_command << service.received_response;
	
	service.ls("", "", "");
	cout << service.last_sent_command << service.received_response;

	service.tree("");
	cout << service.last_sent_command << service.received_response;

	service.logout();
	cout << service.last_sent_command << service.received_response;

	service.closeConnection();
	return 0;
}
