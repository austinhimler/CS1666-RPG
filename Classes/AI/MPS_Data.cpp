#include "../../Headers/AI/MPS_Data.h"

/* 

Implement this function to read modifier for Enemy's own current status

void readAISelfModifierBaseValue(std::string path) {

}

//*/

void MPS_Data::readData(std::string path) {
	std::ifstream data(path, std::ifstream::in);
	if (!data.is_open()) {
		std::cerr << "Failed to open data!" << std::endl;
	}
	else {
		std::string tempString;
		int i = 0;
		while (std::getline(data, tempString)) {
			std::string::size_type Pos = tempString.find(',', 0);
			if (Pos == std::string::npos) {
				std::cerr << "Comma not found at line " << i << std::endl;
				i++;
				continue;
			}
			// Check if newline character is attached at the end, if so, remove it
			std::string::size_type tempPos = tempString.find('\n', Pos);
			if (tempPos != std::string::npos) {
				tempString.erase(tempPos);
			}
			Data.push_back(std::stoi(tempString.substr(Pos + 1)));
			i++;
		}
		if (i != Length) {
			std::cerr << "Only " << i << " rows of data are read, but the right number should be " << Length << std::endl;
		}
		data.close();
	}
}

MPS_Data::MPS_Data(std::string path, int i) {
	Length = i;
	readData(path);
}

std::vector<int> MPS_Data::getIntData() {
	std::vector<int> TBP;
	for (auto& d : Data) {
		TBP.push_back((int)d);
	}
	return TBP;
}

std::vector<float> MPS_Data::getData() {
	return Data;
}