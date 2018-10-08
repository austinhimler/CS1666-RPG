#include "Headers/Combat.h"
#include "Headers/QueueManager.h"
#include "Headers/Character.h"
#include "Headers/Player.h"
#include "Headers/Enemy.h"


Combat::Combat()
{
}


Combat::~Combat()
{
}

public void combat() 
{
	/**
	*	Combat Manager - Start Battle:
						get participants
						display combat UI
						pre-combat effect calcuation
						create queue
						start pre-turn of first from queue
						*/

	// For now creating player and enemy within combat class. Could be subject to change 
	Player p1;
	Enemy e1;
	// Set up a Character array and populate it (not sorted by dex) 
	Character participants[2];
	participants[0] = p1;
	participants[1] = e1;
	// Create QueueManager obj which contains sorting of participant array. 
	QueueManager qm;
	// This will populate two arrays representing current turn. currTurn and nextTurn 
	qm.createTurns(participants);
	while (p1.hpCurrent > 0 && e1.hpCurrent > 0) {
		//Goes thru pre turn -> in turn -> post turn
		pre_turn();
		// Changes the turn / reorganizes array
		qm.changeTurns();
	}


}
public void pre_turn() {
	/**
	*	Turn - pre-turn:
						pre-turn effect calculation (if we wanna add status effects that carry over from one battle to the next we can but for now let's just make all effects wiped at the end of combat)
						check survival
						start in-turn
						*/
	if (p1.hpCurrent <= 0) {
		// game over
		exit(0);
	}
	// Game on!
	else {
		in_turn();
	}
}
public void in_turn() {
	/**
			Turn - in-turn:
							display UI
							loop
								accept actions
								calculate effect
								check survival
							start end-turn
		*/

		// Right now character array is just 2 so we just goin thru it for player and enemy
	for (int i = 0; i < currTurn.size(); i++) {
		// take your turn. attack someone, check if they're dead, etc.
		if (dynamic_cast<currTurn[i]> (Player)) {
			// take player inputs, its a player obj in the array
		}
		else {
			// This is the enemies' turn
		}
		if (p1.hpCurrent <= 0) {
			exit(0);
		}
		if (e1.hpCurrent <= 0) {
			//Do some end battle shit
		}
	}
	end_turn();
}
public void end_turn() {
	/**
		Turn - end_turn:
						end-turn calculation
						check surivval
						call combat manager for next turn
	*/
	// Do calculations, check if enemies are dead, etc.
	if (e1.hpCurrent <= 0) {
		//Do some end battle shit
	}
	if (p1.hpCurrent <= 0) {
		exit(0);
	}
}
