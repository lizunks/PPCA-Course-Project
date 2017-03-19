#pragma once
#include "datatype.h"
#include "number.h"
#include "boolean.h"
#define SCAST_CHARACTER(x) static_cast<Character*>(x)

class Character :public Datatype{
public:
	Character(char ch = '\0');
	~Character();
	virtual Boolean *charIsEqual(Character *char2);
	virtual Boolean *charIsCiEqual(Character *char2);
	virtual Boolean *charLess(Character *char2);
	virtual Boolean *charCiLess(Character *char2);
	virtual Boolean *charLessequal(Character *char2);
	virtual Boolean *charCiLessequal(Character *char2);
	virtual Boolean *charIsAlpha();
	virtual Boolean *charIsNum();
	virtual Boolean *charIsSpace();
	virtual Boolean *charIsUpCase();
	virtual Boolean *charIsLowCase();
	virtual Character *charToUpCase();
	virtual Character *charToDownCase();

	virtual Datatype *charToInt();

	virtual Boolean *isInteger();
	virtual Boolean *isRational();
	virtual Boolean *isReal();
	virtual Boolean *isComplex();
	virtual Boolean *isNumber();
	virtual Boolean *isChar();
	virtual Boolean *isString();
	virtual void print();
	static Character *from_string(const char*expression);
	
	char char_;
};