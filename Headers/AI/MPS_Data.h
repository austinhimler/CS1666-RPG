#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "CombatAIResource.h"

class MPS_Data {
protected:
	std::vector<float> Data;
	int Length;

	//void readAISelfModifierBaseValue(std::string path);
	void readData(std::string path);

public:
	/** constructors **/
	MPS_Data(std::string path, int length);

	/** getters **/
	std::vector<int> getIntData();
	std::vector<float> getData();
};