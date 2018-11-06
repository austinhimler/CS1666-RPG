#ifndef _____ATTR_GLOBALS_H_____
#define _____ATTR_GLOBALS_H_____
#include <string>


enum attr_index {
	STR,
	INT,
	DEX,
	CON,
	FAI,
	ATTRCOUNT
};

const std::string ATTR_NAMES[] = {
	"Strength",
	"Intelligence",
	"Dexerity",
	"Constitution",
	"Faith"
};
#endif