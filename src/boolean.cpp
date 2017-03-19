#include "boolean.h"
#include <cstdio>
#include <cstring>

Boolean::Boolean(bool value) :value_(value){
	type_ = BOOLEAN;
}

Boolean::~Boolean(){}

Boolean *Boolean::isEqual(Boolean *boolean2){
	return new Boolean(value_ == boolean2->value_);
}

Boolean *Boolean::isInteger(){
	return new Boolean(false);
}

Boolean *Boolean::isRational(){
	return new Boolean(false);
}

Boolean *Boolean::isReal(){
	return new Boolean(false);
}

Boolean *Boolean::isComplex(){
	return new Boolean(false);
}

Boolean *Boolean::isNumber(){
	return new Boolean(false);
}

Boolean *Boolean::isChar(){
	return new Boolean(false);
}

Boolean *Boolean::isString(){
	return new Boolean(false);
}

void Boolean::print(){
	if (value_) printf("#t");
	else printf("#f");
}

Boolean *Boolean::from_string(const char *expression){
	if (strcmp(expression, "#t") == 0 || strcmp(expression, "#T") == 0) return new Boolean(true);
	if (strcmp(expression, "#f") == 0 || strcmp(expression, "#F") == 0) return new Boolean(false);
	else return NULL;
}
