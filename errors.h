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
	BAD_TOKEN_USE = 394,
	RESERVED_NAME = 395,
	INVALID_CONTEXT = 396,
	CONST_VIOLATION = 397,
};

struct Position {
	unsigned int line;
	unsigned int col;
	const char* file;
	Position(int line, int col);
	Position() {}

	bool operator==(Position);
};

void ERROR(error_type t, 
	const char* message, 
	Position pos, 
	const char* FILE, 
	std::string& code
);


#endif //ERRORS_CF_H