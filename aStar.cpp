#include "../Headers/aStar.h"

class Node {
public:
	int id;
	float cost;
	Node(int i, float c) : id(i), cost(c) {}
};
bool operator<(const Node& n1, const Node& n2) {
	return n1.cost > n2.cost;
}
bool operator==(const Node& n1, const Node& n2) {
	return n1.id == n2.id;
}
//diagonal distance
float linf_norm(int a, int b, int c, int d) {
	return std::max(std::abs(a - c), std::abs(b - d));
}
//linear distance
float l1_norm(int a, int b, int c, int d) {
	return std::abs(a - c) + std::abs(b - d);
}
/*
weight: h x w grid of costs
h, w: height and width of grid (30 x 30)
*/
bool aStar::aStarAlgo(int start, int goal) {
	const float infinity = std::numeric_limits<float>::infinity();
	Node startNode(start, 0.);
	Node goalNode(goal, 0.);

	float* costs = new float[h * w];
	for (int i = 0; i < h * w; ++i)
		costs[i] = infinity;
	costs[start] = 0.;

	std::priority_queue<Node> visit;
	visit.push(startNode);

	int* neighbors = new int[8];
	bool isSolution = false;
	while (!visit.empty()) {
		Node current = visit.top();

		if (current == goalNode) {
			isSolution = true;
			break;
		}
		visit.pop();
		int row = current.id / w;
		int col = current.id % w;
		neighbors[0] = (diag && row > 0 && col > 0) ? current.id - w - 1 : -1;
		neighbors[1] = (row > 0) ? current.id - w : -1;
		neighbors[2] = (diag && row > 0 && col + 1 < w) ? current.id - w + 1 : -1;
		neighbors[3] = (col > 0) ? current.id - 1 : -1;
		neighbors[4] = (col + 1 < w) ? current.id + 1 : -1;
		neighbors[5] = (diag && row + 1 < h && col > 0) ? current.id + w - 1 : -1;
		neighbors[6] = (row + 1 < h) ? current.id + w : -1;
		neighbors[7] = (diag && row + 1 < h && col + 1 < w) ? current.id + w + 1 : -1;

		float hCost;
		for (int i = 0; i < 8; ++i) {
			if (neighbors[i] >= 0) {
				float newCost = costs[current.id] + weights[neighbors[i]];
				if (newCost < costs[neighbors[i]]) {
					if (diag) {
						hCost = linf_norm(neighbors[i] / w, neighbors[i] % w, goal / w, goal % w);
					}
					else {
						hCost = l1_norm(neighbors[i] / w, neighbors[i] % w, goal / w, goal % w);
					}
					float priority = newCost + hCost;
					visit.push(Node(neighbors[i], priority));
					costs[neighbors[i]] = newCost;
					seq.push(current.id);
				}
			}
		}
	}
	delete[] costs;
	delete[] neighbors;
	return isSolution;
}