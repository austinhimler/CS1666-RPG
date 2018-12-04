#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <cstdio>

const int n = 30;
const int m = 30;
static int mapping[n][m];
static int closed[n][m];
static int open[n][m];
static int dir_map[n][m];
const int direction = 8;
static int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

class  Node {
	//30 x 30 map of Nodes
	int xValue;
	int yValue;
	int value;
	int pri;
public:
	Node() {
		xValue = -1;
		yValue = -1;
		value = -1;
		pri = -1;
	}
	Node(int x, int y, int v, int p) {
		xValue = x;
		yValue = y;
		value = v;
		pri = p;
	}
	int getX() const { return xValue; }
	int getY() const { return xValue; }
	int getValue()const { return value; }
	int getPri() const { return pri; }
	int updatePri(const int& x, const int& y) {
		pri = value + estimate(x, y) * 10;
		return pri;
	}
	void nextLevel(const int & a) {
		value += (direction == 8 ? (a % 2 == 0 ? 10 : 14) : 10);
	}
	const int& estimate(const int& x, const int& y) const {
		static int xDist, yDist, dist;
		xDist = x - xValue;
		yDist = y - yValue;
		return static_cast<int>(sqrt(xDist * xDist + yDist * yDist));
	}
};

bool operator<(const Node& a, const Node& b) {
	return a.getPri() > b.getPri();
}

std::string pathFindAlgo(const int& x0, const int& y0, const int& x1, const int& y1) {
	static std::priority_queue<Node> pQueue[2];
	static int pqIndex;
	static Node* n0;
	static Node* m0;
	static int i, j, x, y, xdx, ydy;
	static char c;
	pqIndex = 0;
	for (y = 0; y < m; y++) {
		for (x = 0; x < n; x++) {
			closed[x][y] = 0;
			open[x][y] = 0;
		}
	}

	n0 = new Node(x0, y0, 0, 0);
	n0->updatePri(x1, y1);
	pQueue[pqIndex].push(*n0);
	open[x0][y0] = n0->getPri();
	delete n0;

	while (!pQueue[pqIndex].empty()) {
		std::cout << "x";
		n0 = new Node(pQueue[pqIndex].top().getX(),
			pQueue[pqIndex].top().getY(),
			pQueue[pqIndex].top().getValue(),
			pQueue[pqIndex].top().getPri());
		x = n0->getX();
		y = n0->getY();

		pQueue[pqIndex].pop();
		open[x][y] = 0;
		closed[x][y] = 1;

		if (x == x1 && y == y1) {
			std::cout << "2";
			std::string path = "";
			while (!(x == x0 && y == y0)) {
				j = dir_map[x][y];
				c = '0' + (j + direction / 2) % direction;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}
			delete n0;
			while (!pQueue[pqIndex].empty())
				pQueue[pqIndex].pop();
			return path;
		}
		//std::cout << "2";
		for (i = 0; i < direction; i++) {
			//std::cout << "3";
			xdx = x + dx[i];
			ydy = y + dy[i];
			if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 || mapping[xdx][ydy] == 1 || closed[xdx][ydy] == 1)) {
				m0 = new Node(xdx, ydy, n0->getValue(), n0->getPri());
				m0->nextLevel(i);
				m0->updatePri(x1, y1);
				if (open[xdx][ydy] == 0) {
					open[xdx][ydy] = m0->getPri();
					pQueue->push(*m0);
					delete m0;
					dir_map[xdx][ydy] = (i + direction / 2) % direction;
				}
				else if (open[xdx][ydy] > m0->getPri()) {
					open[xdx][ydy] = m0->getPri();
					dir_map[xdx][ydy] = (i + direction / 2) % direction;
					//std::cout << "4";
					while (!(pQueue[pqIndex].top().getX() == xdx && pQueue[pqIndex].top().getY() == ydy)) {
						//std::cout << "8";
						pQueue[1 - pqIndex].push(pQueue[pqIndex].top());
						//std::cout << "9";
						if (pQueue[pqIndex].empty())
							pQueue[pqIndex].pop();
					}
					//std::cout << "5";
					pQueue[pqIndex].pop();
					if (pQueue[pqIndex].size() > pQueue[1 - pqIndex].size())
						pqIndex = 1 - pqIndex;
					while (!pQueue[pqIndex].empty()) {
						pQueue[1 - pqIndex].push(pQueue[pqIndex].top());
						pQueue[pqIndex].pop();
					}
					pqIndex = 1 - pqIndex;
					pQueue[pqIndex].push(*m0);
					delete m0;
				}
				else delete m0;
			}
		}
		delete n0;
	}
	return "";
}

int main() {
	std::srand(time(NULL));
	for (int y = 0; y < m; y++) {
		for (int x = 0; x < n; x++) {
			mapping[x][y] = 0;
		}
	}

	for (int x = n / 8; x < n * 7 / 8; x++) {
		mapping[x][m / 2] = 1;
	}
	for (int y = m / 8; y < m * 7 / 8; y++) {
		mapping[n / 2][y] = 1;
	}
	int xA, xB, yA, yB;
	switch (rand() % 8)
	{
	case 0: xA = 0; yA = 0; xB = n - 1; yB = m - 1; break;
	case 1: xA = 0; yA = m - 1; xB = n - 1; yB = 0; break;
	case 2: xA = n / 2 - 1; yA = m / 2 - 1; xB = n / 2 + 1; yB = m / 2 + 1; break;
	case 3: xA = n / 2 - 1; yA = m / 2 + 1; xB = n / 2 + 1; yB = m / 2 - 1; break;
	case 4: xA = n / 2 - 1; yA = 0; xB = n / 2 + 1; yB = m - 1; break;
	case 5: xA = n / 2 + 1; yA = m - 1; xB = n / 2 - 1; yB = 0; break;
	case 6: xA = 0; yA = m / 2 - 1; xB = n - 1; yB = m / 2 + 1; break;
	case 7: xA = n - 1; yA = m / 2 + 1; xB = 0; yB = m / 2 - 1; break;
	}

	std::cout << "Map Size (X,Y): " << n << "," << m << std::endl;
	std::cout << "Start: " << xA << "," << yA << std::endl;
	std::cout << "Finish: " << xB << "," << yB << std::endl;
	// get the route
	clock_t start = clock();
	std::string route = pathFindAlgo(xA, yA, xB, yB);
	if (route == "") std::cout << "An empty route generated!" << std::endl;
	clock_t end = clock();
	double time_elapsed = double(end - start);
	std::cout << "Time to calculate the route (ms): " << time_elapsed << std::endl;
	std::cout << "Route:" << std::endl;
	std::cout << route << std::endl << std::endl;

	// follow the route on the map and display it 
	if (route.length() > 0)
	{
		int j; char c;
		int x = xA;
		int y = yA;
		mapping[x][y] = 2;
		for (int i = 0; i < route.length(); i++)
		{
			c = route.at(i);
			j = atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			mapping[x][y] = 3;
		}
		mapping[x][y] = 4;

		// display the map with the route
		for (int y = 0; y < m; y++)
		{
			for (int x = 0; x < n; x++)
				if (mapping[x][y] == 0)
					std::cout << ".";
				else if (mapping[x][y] == 1)
					std::cout << "O"; //obstacle
				else if (mapping[x][y] == 2)
					std::cout << "S"; //start
				else if (mapping[x][y] == 3)
					std::cout << "R"; //route
				else if (mapping[x][y] == 4)
					std::cout << "F"; //finish
			std::cout << std::endl;
		}
	}
	getchar(); // wait for a (Enter) keypress  
	return(0);
}
