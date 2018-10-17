#pragma once

#include <string>
#include <vector>

class Inventory {
	public:
		Inventory();
		Inventory(int maxLoad, int curLoad);
	private:
		int maxLoad;
		int curLoad;
};