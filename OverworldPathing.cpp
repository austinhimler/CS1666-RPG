#include "../Headers/OverworldPathing.h"


OverworldPathing::OverworldPathing() {
	for (int a = 0; a < TOTAL_TILES; a++) {
		OverworldPathing::mapping[a] = 0;
	}
}

OverworldPathing::OverworldPathing(Tile *map[900]) {
	for (int a = 0; a < TOTAL_TILES; a++) {
		OverworldPathing::mapping[a] = map[a]->mType;
	}
}

bool OverworldPathing::isValid(int x, int y) {
	if (x < 0 || y < 0 || x > SCREEN_WIDTH / TILE_WIDTH || y > SCREEN_HEIGHT / TILE_HEIGHT) return false;
	int id = (x + 144 / 2) / TILE_WIDTH;
	id += (y + 144) / TILE_HEIGHT;
	if (OverworldPathing::mapping[id] == 0) return true;
	else return false;
}

bool OverworldPathing::isDestination(int x, int y, Node dest) {
	if (x == dest.x && y == dest.y) {
		return true;
	}
	else
		return false;
}

double OverworldPathing::calculateH(int x, int y, Node dest) {
	return (sqrt((x - dest.x) * (x - dest.x) + (y - dest.y)*(y - dest.y)));
}

std::vector<Node> OverworldPathing::aStar(Node p, Node dest) {
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
	while (!open.empty() && open.size() < 900) {
		Node n;
		do {
			float temp = FLT_MAX;
			
			std::vector<Node>::iterator itNode;
			for (std::vector<Node>::iterator it = open.begin(); it != open.end(); it = next(it)) {
				Node node = *it;
				if (node.fCost < temp) {
					temp = node.fCost;
					itNode = it;
				}
			}

			
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

std::vector<Node> OverworldPathing::makePath(std::array<std::array<Node, 30>, 30> map, Node dest) {
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

void OverworldPathing::pathSequence(Cluster* c, Player* p) {
	int tile = (int)(p->xPosition + (p->rectangle.w / 2)) / TILE_WIDTH;
	tile += (int)((p->yPosition + p->rectangle.h) / TILE_HEIGHT) * 30;

	if (tile != playerTile) {
		Node npc, player;
		npc.x = c->xPosition;
		npc.y = c->yPosition;
		player.x = p->xPosition;
		player.y = p->yPosition;

		//aStar(npc, player);
	} 
}

void OverworldPathing::updatePlayerLocation(Player* p) {
	int tile = (int)(p->xPosition + (p->rectangle.w / 2)) / TILE_WIDTH;
	tile += (int)((p->yPosition + p->rectangle.h) / TILE_HEIGHT) * 30;

	playerTile = tile;
}

void OverworldPathing::test(Cluster* c, Player* p) {

}

int OverworldPathing::getDirection(Cluster* c, Player* p) {
	Node npc, player;
	npc.x = c->xPosition/TILE_WIDTH;
	npc.y = c->yPosition/TILE_HEIGHT;
	player.x = p->xPosition/TILE_WIDTH;
	player.y = p->yPosition/TILE_HEIGHT;

	Node temp = aStar(npc, player)[0];
	Node dest;
	dest.x = temp.x;// path[0].x;
	dest.y = temp.y;// path[0].y;

	/*
	if dest-x is greater and dest-y is greater:	right or down (random)
					 ... and dest-y is less: up or right (random)
.				     ... and dest-y is equal: right
	if dest-x is equal and dest-y is greater: down
				   ... and dest-y is less: up
				   ... and dest-y is equal: (@ destination Node) error with pathing algo
	if dest-x is less and dest-y is greater: down or left (random)
				  ... and dest-y is less: left or up (random)
				  ... and dest-y is equal: left
	*/
	if (dest.x > c->xPosition / TILE_WIDTH) {
		if (dest.y > c->yPosition / TILE_HEIGHT) {
			int temp = rand() % 2;
			if (temp) return 2;
			else return 3;
		}
		else if (dest.y < c->yPosition / TILE_HEIGHT) {
			int temp = rand() % 2;
			if (temp) return 0;
			else return 3;
		}
		else if (dest.y == c->yPosition / TILE_HEIGHT) return 3;
	}
	else if (dest.x == c->xPosition / TILE_WIDTH) {
		if (dest.y > c->yPosition / TILE_HEIGHT) return 2;
		else if (dest.y < c->yPosition / TILE_HEIGHT) return 0;
		else if (dest.y == c->yPosition / TILE_HEIGHT) {
			return -1;
		}
	}
	else if (dest.x < c->xPosition) {
		if (dest.y > c->yPosition) {
			int temp = rand() % 2;
			if (temp) return 1;
			else return 2;
		}
		else if (dest.y < c->yPosition) {
			int temp = rand() % 2;
			if (temp) return 0;
			else return 1;
		}
		else if (dest.y == c->yPosition) return 1;
	}
}
