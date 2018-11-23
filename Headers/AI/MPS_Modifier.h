#pragma once

class MPS_Modifier {
protected:
	int Type;
	float Val;
public:
	virtual int getType() = 0;
	virtual float getVal() = 0;
};