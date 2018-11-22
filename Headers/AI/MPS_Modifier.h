#pragma once

class MPS_Modifier {
protected:
	int Type;
	float Val;
public:
	MPS_Modifier();
	virtual int getType();
	virtual float getVal();
};