#ifndef _____INVENTORY_H_____
#define _____INVENTORY_H_____

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

#endif