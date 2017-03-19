#pragma once

class Datatype{
public:
	enum{
		RATIONAL = 1,
		FLOAT = 2,
		COMPLEX = 3,
		BOOLEAN = 4,
		CHARACTER = 5,
		STRING = 6,
	} type_;
	Datatype(){}
	virtual ~Datatype(){};
	virtual Datatype *isInteger() = 0;
	virtual Datatype *isRational() = 0;
	virtual Datatype *isReal() = 0;
	virtual Datatype *isComplex() = 0;
	virtual Datatype *isNumber() = 0;
	virtual Datatype *isChar() = 0;
	virtual Datatype *isString() = 0;
	virtual void print() = 0;
};

