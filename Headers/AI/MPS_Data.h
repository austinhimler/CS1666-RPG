#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "CombatAIResource.h"

class MPS_Data {
protected:
	std::vector<int> Data;

public:

	MPS_Data(std::string path);

	/** getters **/
	std::vector<int> getData();
};