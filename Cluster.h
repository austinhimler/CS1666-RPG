
#ifndef _____CLUSTER_H_____
#define _____CLUSTER_H_____
#include <string>
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include <map>
#include <random>
#include <iostream>

class Cluster : public Character
{
public:
	Cluster();
	Cluster(int q);
	Cluster(std::string n);
	Cluster(std::string n, int q);
	/*
	Cluster(Enemy e, int q);
	*/
	vector<Character*> characterGroup;
	int clusterSize;
	bool alive;

	void setAiOptions(int movementType, double range, Player* p) {
		this->movementType = movementType;
		this->range = range;
		this->ptr = p;
	}

	std::string getMove() {
		if (movementType < 0 || range < 0) return "null";
		else {
			generateDecisions();
			if (movementType == 0) return getAgroDecision();
			else if (movementType == 1) return getPassiveDecision();
			else if (movementType == 2) return getRandomDecision();
		}
	}

private:
	/*
	0: (agressive) npc will move toward player (if player currently w/i range)
		when player out of range, NPC moves back toward spawn (not implemented)

	1: (passive) npc will run away from player (if player gets w/i range)
		when player out of range, NPC sets new spawn (not implemented)

	2: (random) npc decides to move in random direction
	*/
	int movementType;
	double range;

	Player* ptr;

	map <std::string, double> decisions;

	void generateDecisions() {
		//up
		decisions.insert(pair <std::string, double>("up", calcDistance(this->xPosition, this->yPosition - 1.0, ptr->xPosition, ptr->yPosition)));
		//right
		decisions.insert(pair <std::string, double>("right", calcDistance(this->xPosition + 1.0, this->yPosition, ptr->xPosition, ptr->yPosition)));
		//down
		decisions.insert(pair <std::string, double>("down", calcDistance(this->xPosition, this->yPosition + 1.0, ptr->xPosition, ptr->yPosition)));
		//left
		decisions.insert(pair <std::string, double>("left", calcDistance(this->xPosition - 1.0, this->yPosition , ptr->xPosition, ptr->yPosition)));
	}

	/*
	compare distance (b/t npc and player) that would result from different
	choices
	
	looking for choice with shortest resulting distance (agro)
	 
	goto sleep if player out of range
	*/
	std::string getAgroDecision() {
		double temp = calcDistance(this->xPosition, this->yPosition, ptr->xPosition, ptr->yPosition);
		//if distance to player is greater than range, goto sleep
		if (temp <= range) return "sleep";
		else {
			/*
			o/w parse all possible decisions to minimize distance
			b/t player and npc
			*/
			std::string result;
			map <std::string, double> :: iterator itr;
			for (itr = decisions.begin(); itr != decisions.end(); ++itr) {
				if (itr->second < temp) {
					result = itr->first;
					temp = itr->second;
				}
			}
			return result;
		}
	}

	/*
	compare distance (b/t npc and player) that would result from
	various possible decision-choices
	
	look for choice with largest resulting distance (passive)

	goto sleep if player out of range
	*/
	std::string getPassiveDecision() {
		double temp = calcDistance(this->xPosition, this->yPosition, ptr->xPosition, ptr->yPosition);
		//std::cout << ptr->xPosition;
		//if distance to player is greater than range, goto sleep
		if (temp <= range) return "sleep";
		else {
			/*
			o/w parse all possible decisions to maximize distance
			b/t player and npc
			*/
			std::string result;
			map <std::string, double> ::iterator itr;
			for (itr = decisions.begin(); itr != decisions.end(); ++itr) {
				if (itr->second > temp) {
					result = itr->first;
					temp = itr->second;
				}
			}
			return result;
		}
	}

	/*
	randomly choose direction/sleep

	npc will move up, down, right, left, or idle(sleep)
	regardless of current distance to player
	*/
	std::string getRandomDecision() {
		//std::cout << range << std::endl;
		int counter = 0;
		int temp = rand() % 5;
		if (temp == 4) return "sleep";
		else {
			map <std::string, double> ::iterator itr;
			for (itr = decisions.begin(); itr != decisions.end(); ++itr) {
				if (temp == counter) return itr->first;
				counter++;
			}
		}
	}

	double calcDistance(double x0, double y0, double x1, double y1) { 
		return (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1); 
	}
};
#endif