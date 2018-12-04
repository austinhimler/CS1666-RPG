#ifndef _____MPS_MODIFIER_H_____
#define _____MPS_MODIFIER_H_____



class MPS_Modifier {
protected:
	int Type;
	float Val;
public:
	virtual ~MPS_Modifier() = 0;
	virtual int getType() = 0;
	virtual float getVal() = 0;
};

inline MPS_Modifier::~MPS_Modifier(){}


#endif // 
