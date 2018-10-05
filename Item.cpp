#include "Headers/Item.h"

Item::Item() {
	this->description = "item description";
	this->name = "item name";
	this->itemType = 0;
};
Item::Item(std::string name, std::string description, int itemType) {
	this->name = name;
	this->description = description;
	if (itemType < 0 || itemType > 3) itemType = 0;
	this->itemType = itemType;
}
