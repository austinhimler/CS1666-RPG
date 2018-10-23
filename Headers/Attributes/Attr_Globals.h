#pragma once
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
