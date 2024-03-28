#ifndef ERRORS_CF_H
#define ERRORS_CF_H
#include <string>

enum class error_type {
	INCOMPLETE_STRING = 388,
	UNEXPECTED_TOKEN = 389,
	IMPORTED_FILE_NOT_FOUND = 390,
	NOT_IN_GLOBAL_SCOPE = 391,
	TOKEN_NOT_DEFINED = 392,
	REPEATED_DEFINITION = 393,
};

struct Position {
	unsigned int line;
	unsigned int col;
	Position(int line, int col);
	Position() {}
};

void ERROR(error_type t, 
	const char* message, 
	Position pos, 
	const char* FILE, 
	std::string& code
);


#endif //ERRORS_CF_H