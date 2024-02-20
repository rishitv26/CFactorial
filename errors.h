#ifndef ERRORS_CF_H
#define ERRORS_CF_H
#include <string>

enum class error_type {
	INCOMPLETE_STRING = 0,
};

struct Position {
	unsigned int line;
	unsigned int col;
	Position(int line, int col);
};

void ERROR(error_type t, 
	const char* message, 
	Position pos, 
	const char* FILE, 
	std::string& code
);


#endif //ERRORS_CF_H