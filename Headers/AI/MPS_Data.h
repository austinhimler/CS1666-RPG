#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "CombatAIResource.h"

class MPS_Data {
protected:
	std::vector<float> Data;

	//void readAISelfModifierBaseValue(std::string path);
	void readTaskBasePriority(std::string path);

public:
	/** constructors **/
	MPS_Data(std::string path);

	/** getters **/
	std::vector<int> getTBP();
	std::vector<float> getData();
};