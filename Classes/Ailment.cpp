#include "../Headers/Ailment.h"
#include <Random>

Ailment::Ailment(int n, double c, int v, int tn, int sa, int d) {
	name = n;
	chance = c;
	value = v;
	turnN = tn;
	stopAction = sa;
	degree = d;
}

Ailment::Ailment(int n, int f1, int f2, Ability a) {
	name = n;
	value = a.getVal() / 3 + degree * 10;
	if(n)
	degree = std::rand() % 100;
	if (degree < 30) degree = 1;
	else if (degree < 55) degree = 2;
	else if (degree < 75) degree = 3;
	else if (degree < 90) degree = 4;
	else if (degree < 100) degree = 5;
	chance = (f1 - f2) * AilmentResource::ailmentCompen[name];
	if (chance <= 10) chance = 10;
	turnN = chance / 3;
	stopAction = AilmentResource::ailmentSA[n];
	
}

Ailment::Ailment(int n, double c) {
	name = n;
	chance = c;
}