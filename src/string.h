#pragma once
#include "datatype.h"
#include "number.h"
#include "boolean.h"
#include "character.h"
#include <string>
using namespace std;
#define SCAST_STRING(x) static_cast<String*>(x)

class String :public Datatype{
public:
	String(string str="");
	~String();
	virtual Boolean *strIsEqual(String *string2);
	virtual Boolean *strIsCiEqual(String *string2);
	virtual Boolean *strLess(String *string2);
	virtual Boolean *strCiLess(String *string2);
	virtual Boolean *strLessequal(String *string2);
	virtual Boolean *strCiLessequal(String *string2);

	virtual Datatype *strLen();
	virtual Datatype *strRef(Number *number);
	virtual String *subStr(Number *start, Number *end);
	virtual String *strAppend(String *string2);
	virtual String *strCopy();
	virtual String *generateStr(Character *ch);

	virtual Datatype *strToNum();

	virtual Boolean *isInteger();
	virtual Boolean *isRational();
	virtual Boolean *isReal();
	virtual Boolean *isComplex();
	virtual Boolean *isNumber();
	virtual Boolean *isChar();
	virtual Boolean *isString();
	virtual void print();
	static String *from_string(const char*expression);
	static String *makeStr(Number *number, Character *ch);

	string string_;
};