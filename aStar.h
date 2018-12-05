#ifndef _____aStar_H_____
#define _____aStar_H_____

#include <queue>
#include <limits>
#include <cmath>


class aStar {
public:
	aStar() {
	};
	bool aStarAlgo(int start, int goal);
	bool diag = false;
	int h = 64;
	int w = 64;

	std::queue<int> seq; 
	float* weights;

	int lastPlayerLocation = 0;
};

#endif