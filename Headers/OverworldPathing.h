#ifndef _____OverworldPathing_H_____
#define _____OverworldPathing_H_____
#include <string>
#include <vector>
#include <math.h>
#include <stack>
#include <array>
#include <exception>
#include "Tile.h"
#include "Globals.h"

struct Node {
	int x;
	int y;
	int parentX;
	int parentY;
	float gCost;
	float hCost;
	float fCost;
};

inline bool operator < (const Node& a, const Node& b) {
	return a.fCost < b.fCost;
}

class OverworldPathing {
public:
	OverworldPathing(Tile *m[900]) {
		for (int a = 0; a < TOTAL_TILES; a++) {
			map[a] = m[a];
		}
	}
private:
	static Tile* map[900];
	/*
	int currentTile = (int)(player1.xPosition + (player1.rectangle.w / 2)) / TILE_WIDTH;
	currentTile += (int)((player1.yPosition + player1.rectangle.h) / TILE_HEIGHT) * 30;

	*/
	static bool isValid(int x, int y) {
		if (x < 0 || y < 0 || x > SCREEN_WIDTH/TILE_WIDTH || y > SCREEN_HEIGHT/TILE_HEIGHT) return false;
		int id = (x + 144 / 2) / TILE_WIDTH;
		id += (y + 144) / TILE_HEIGHT;
		if (map[id]->mType == 0) return true;
		else return false;
	}
	static bool isDestination(int x, int y, Node dest) {
		if (x == dest.x && y == dest.y) {
			return true;
		}
		else
			return false;
	}
	static double calculateH(int x, int y, Node dest) {
		return (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y)*(y - dest.y)));
	}
	static std::vector<Node> aStar(Node p, Node dest) {
		std::vector<Node> empty;
		if (isValid(dest.x, dest.y) == false) {
			return empty;
		}
		if (isDestination(p.x, p.y, dest)) {
			return empty;
		}
		bool closed[30][30];
		std::array<std::array<Node, 30>, 30> all;
		for (int x = 0; x < 30; x++) {
			for (int y = 0; y < 30; y++) {
				all[x][y].fCost = FLT_MAX;
				all[x][y].gCost = FLT_MAX;
				all[x][y].hCost = FLT_MAX;
				all[x][y].parentX = -1;
				all[x][y].parentY = -1;
				all[x][y].x = x;
				all[x][y].y = y;
				closed[x][y] = false;
			}
		}
		int x = p.x;
		int y = p.y;
		all[x][y].fCost = 0.0;
		all[x][y].gCost = 0.0;
		all[x][y].hCost = 0.0;
		all[x][y].parentX = x;
		all[x][y].parentY = y;
		std::vector<Node> open;
		open.emplace_back(all[x][y]);
		bool found = false;
		while (!open.empty && open.size() < 900) {
			Node n;
			do {
				float temp = FLT_MAX;
				std::vector<Node>::iterator it;
				for (std::vector<Node>::iterator i = open.begin(); i != open.end(); i = next(i)) {
					Node node = *i;
					if (node.fCost < temp) {
						temp = node.fCost;
						it = i;
					}
				}
				n = *it;
				open.erase(it);
			} while (isValid(n.x, n.y) == false);
			x = n.x;
			y = n.y;
			closed[x][y] = true;
			for (int a = -1; a <= 1; a++) {
				for (int b = -1; b <= 1; b++) {
					double g, h, f;
					if (isValid(x + a, y + b)) {
						if (isDestination(x + a, y + b, dest)) {
							all[x + a][y + b].parentX = x;
							all[x + a][y + b].parentY = y;
							found = true;
							return makePath(all, dest);
						}
						else if (closed[x + a][y + b] == false) {
							g = n.gCost + 1.0;
							h = calculateH(x + a, y + b, dest);
							f = g + h;
							if (all[x + a][y + b].fCost == FLT_MAX ||
								all[x + a][y + b].fCost > f) {
								all[x + a][y + b].fCost = f;
								all[x + a][y + b].gCost = g;
								all[x + a][y + b].hCost = h;
								all[x + a][y + b].parentX = x;
								all[x + a][y + b].parentY = y;
								open.emplace_back(all[x + a][y + b]);
							}
						}
					}
				}
			}
			if (found == false) {
				return empty;
			}
		}
	}
	static std::vector<Node> makePath(std::array<std::array<Node, 30>, 30> map, Node dest) {
		try {
			int x = dest.x;
			int y = dest.y;
			std::stack<Node> path;
			std::vector<Node> use;
			while (!(map[x][y].parentX == x && map[x][y].parentY == y) &&
				map[x][y].x != -1 && map[x][y].y != -1) {
				path.push(map[x][y]);
				int x0 = map[x][y].parentX;
				int y0 = map[x][y].parentY;
				x = x0;
				y = y0;
			}
			path.push(map[x][y]);
			while (!path.empty()) {
				Node a = path.top();
				path.pop();
				use.emplace_back(a);
			}
			return use;
		}
		catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
};
#endif
