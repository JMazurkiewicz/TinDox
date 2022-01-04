#ifndef TINDOX_CONSOLE_CLIENT_CONNECTIONERROR_H
#define TINDOX_CONSOLE_CLIENT_CONNECTIONERROR_H

enum ConnectionError {
	NONE = 0,
	E_CON_TO_SERV,
	E_CLOSE_CON,
	E_CLOSED_CON,
	E_SEND_MESSAGE,
	E_READ_MESSAGE,
	E_NOTHING_READ,
	E_RESPONSE,
	OK = 100,
	E_LINE_TOO_LONG = 300,
	E_TOO_MANY_FIELDS = 301,
	E_BAD_FIELD = 302,
	E_BAD_COMMAND = 303,
	E_UNKNOWN = 401,
	E_NOT_LOGGED_IN = 402,
	E_INVALID_FIELD_VALUE = 403,
	E_NOT_FOUND = 404,
	E_TARGET_NOT_FOUND = 405,
	E_NOT_ENOUGH_PERMS = 406,
	E_USER_ALREADY_LOGGED = 407,
	E_INVALID_CREDENTIALS = 408,
	E_FILE_ALREADY_EXISTS = 409
};

#endif //TINDOX_CONSOLE_CLIENT_CONNECTIONERROR_H
