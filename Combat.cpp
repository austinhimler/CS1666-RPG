#include "Combat.h"



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
	*	Turn - pre-turn:
						pre-turn effect calculation
						check survival
						start in-turn
		Turn - in-turn:
						display UI
						loop
							accept actions
							calculate effect
							check survival
						start end-turn
		Turn - end_turn:
						end-turn calculation
						check surivval
						call combat manager for next turn
		Combat Mnager - between - Turn:
						order queue
						next one from queue



						
	*/


}