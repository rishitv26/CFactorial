#include "errors.h"
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::endl;

Position::Position(int _line, int _col): line(_line),col(_col) {}

static std::vector<std::string> splitcode(std::string& code) {
	std::vector<std::string> c = { "" };
	for (int i = 0; i < code.size(); ++i) {
		if (code[i] == '\n') {
			c.push_back("");
		}
		else c.back() += code[i];
	}
	return c;
}

void ERROR(error_type t,
	const char* message,
	Position pos,
	const char* FILE,
	std::string& code
) {
	cout << "Compile Error: CE" << (int)t << endl;
	cout << ">>>> at line " << pos.line << ", column " << pos.col <<
		" of file \"" << FILE << "\": " << endl;

	std::string code_line = splitcode(code)[pos.line-1];
	cout << "\t " << pos.line << " | " << code_line << endl;
	cout << "\t ";
	for (int i = 0; i < (int)log10(pos.line) + 1; ++i) cout << " ";
	for (int i = 0; i < pos.col - 1; ++i) cout << " ";
	cout << "^" << endl;
	cout << message << endl;
	cout << "exiting with code 1" << endl;
	exit(1);
}

