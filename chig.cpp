#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>
using namespace std;

inline int randNum(int min, int max) {
	return rand() % (max - min + 1) + min;
}

void drawing(vector<int> userWhiteBalls, int userRedBall, vector<int> whiteBalls, int redBalls, double prizeMoney) {
	// unordered_set<int> set1(userWhiteBalls.begin(), userWhiteBalls.end());
	vector<int> commonValues;

	for (const int i : whiteBalls) {
		for (const int j : userWhiteBalls) {
			if (i == j) {
				commonValues.push_back(i);
			}
		}
	}
	if (!commonValues.empty()) {
		std::cout << "Common Values: ";
		for (const int commonValue : commonValues) {
			std::cout << commonValue << " ";
		}
		std::cout << "\n";
	}
	else {
		std::cout << "No common values.\n";
	}

	if (commonValues.size() == 1 and userRedBall == redBalls or userRedBall == redBalls) {
		prizeMoney += 4;
	}
	else if (commonValues.size() == 2 and userRedBall == redBalls or commonValues.size() == 3) {
		prizeMoney += 7;
	}
	else if (commonValues.size() == 3 and userRedBall == redBalls or commonValues.size() == 4) {
		prizeMoney += 100;
	}
	else if (commonValues.size() == 4 and userRedBall == redBalls) {
		prizeMoney += 50000;
	}
	else if (commonValues.size() == 5) {
		prizeMoney += 1000000;
	}
	else if (commonValues.empty() and userRedBall != redBalls) {
		prizeMoney += 0;
	}
	else {
		prizeMoney = 1000000000;
	}
	cout << prizeMoney << endl;

}

int redBallNum() {
	const int redBall_limit{ 26 };
	int redBall{ randNum(1, redBall_limit) };
	return redBall;
}

vector<int> whiteBallNum() {
	const int whiteBall_limit{ 69 };
	vector<int> wballs;

	for (int i = 0; i < 5; ++i) {
		int whiteBall = randNum(1, whiteBall_limit);
		wballs.push_back(whiteBall);

	} return wballs;
}

int main() {
	time_t seconds;
	time(&seconds);
	srand(seconds);
	vector<int> uwbn = whiteBallNum();
	vector<int> wbn = whiteBallNum();
	int rbn = redBallNum();
	int urbn = redBallNum();
	int ticket{};
	drawing(uwbn, urbn, wbn, rbn, 0);
	/*
		for (int i = 0; i < 5; ++i) {
			cout << wbn[i] << endl;
		}
	*/
	
}