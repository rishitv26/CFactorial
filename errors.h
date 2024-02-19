#ifndef ERRORS_CF_H
#define ERRORS_CF_H

struct Position {
	unsigned int line;
	unsigned int col;
	Position(int line, int col);
};


#endif //ERRORS_CF_H