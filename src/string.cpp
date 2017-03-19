#include "string.h"
#include "rational.h"
#include "float.h"
#include "complex.h"
#include <cstring>
#include <cassert>
#include <iostream>

void intToStr2(string &str, int n){
	if (n / 10 == 0) str += n + '0';
	else{
		intToStr2(str, n / 10);
		str += n % 10 + '0';
	}
}

String::String(string str) :string_(str){
	type_ = STRING;
}

String::~String(){}

Boolean *String::strIsEqual(String *string2){
	return new Boolean(string_ == string2->string_);
}

Boolean *String::strIsCiEqual(String *string2){
	string str1 = "", str2 = "";
	for (int i = 0; i < string_.size(); ++i){
		if (string_[i] >= 'a' && string_[i] <= 'z') str1 += string_[i] - 32;
		else str1 += string_[i];
	}
	for (int i = 0; i < string2->string_.size(); ++i){
		if (string2->string_[i] >= 'a' && string2->string_[i] <= 'z') str2 += string2->string_[i] - 32;
		else str2 += string2->string_[i];
	}
	return new Boolean(str1 == str2);
}

Boolean *String::strLess(String *string2){
	return new Boolean(string_ < string2->string_);
}

Boolean *String::strCiLess(String *string2){
	string str1 = "", str2 = "";
	for (int i = 0; i < string_.size(); ++i){
		if (string_[i] >= 'a' && string_[i] <= 'z') str1 += string_[i] - 32;
		else str1 += string_[i];
	}
	for (int i = 0; i < string2->string_.size(); ++i){
		if (string2->string_[i] >= 'a' && string2->string_[i] <= 'z') str2 += string2->string_[i] - 32;
		else str2 += string2->string_[i];
	}
	return new Boolean(str1 < str2);
}

Boolean *String::strLessequal(String *string2){
	return new Boolean(string_ <= string2->string_);
}

Boolean *String::strCiLessequal(String *string2){
	string str1 = "", str2 = "";
	for (int i = 0; i < string_.size(); ++i){
		if (string_[i] >= 'a' && string_[i] <= 'z') str1 += string_[i] - 32;
		else str1 += string_[i];
	}
	for (int i = 0; i < string2->string_.size(); ++i){
		if (string2->string_[i] >= 'a' && string2->string_[i] <= 'z') str2 += string2->string_[i] - 32;
		else str2 += string2->string_[i];
	}
	return new Boolean(str1 <= str2);
}

Datatype *String::strLen(){
	int len = string_.size();
	string tmp = "";
	intToStr2(tmp, len);
	return new Rational(tmp, "1");
}

Datatype *String::strRef(Number *number){
	int len = string_.size();
	int pos = number->returnInt();
	assert(pos >= 0 && pos <= len - 1 && "Index Out Of Range!");
	return new Character(string_[pos]);
}

String *String::subStr(Number *start, Number *end){
	int len = string_.size();
	int pos1 = start->returnInt(), pos2 = end->returnInt();
	assert(pos1 >= 0 && pos2<=len && pos1<=pos2 && "Index Out Of Range!");
	return new String(string_.substr(pos1, pos2-pos1+1));
}

String *String::strAppend(String *string2){
	return new String(string_.append(string2->string_));
}

String *String::strCopy(){
	return new String(*this);
}

String *String::generateStr(Character *ch){
	string res = string_;
	res += ch->char_;
	return new String(res);
}

Datatype *String::strToNum(){
	int len = string_.size();
	char *str = new char[len + 1];
	for (int i = 0; i < len; ++i){
		str[i] = string_[i];
	}
	str[len] = '\0';
	const char *tmp = str;
	Number *res = Rational::from_string(tmp);
	if (!res) res = Float::from_string(tmp);
	if (!res) res = Complex::from_string(tmp);
	if (!res){
		delete tmp;
		return new Boolean(false);
	}
	else{
		delete tmp;
		return res;
	}
}


Boolean *String::isInteger(){
	return new Boolean(false);
}

Boolean *String::isRational(){
	return new Boolean(false);
}

Boolean *String::isReal(){
	return new Boolean(false);
}

Boolean *String::isComplex(){
	return new Boolean(false);
}

Boolean *String::isNumber(){
	return new Boolean(false);
}

Boolean *String::isChar(){
	return new Boolean(false);
}

Boolean *String::isString(){
	return new Boolean(true);
}

void String::print(){
	cout << "\"";
	for (int i = 0; i < string_.size(); ++i){
		if (string_[i] == '\b') cout << "\\b";
		else if (string_[i] == '\f') cout << "\\f";
		else if (string_[i] == '\n') cout << "\\n";
		else if (string_[i] == '\r') cout << "\\r";
		else if (string_[i] == '\t') cout << "\\t";
		else if (string_[i] == '\v') cout << "\\v";
		else if (string_[i] == '\\') cout << "\\\\";
		else if (string_[i] == '\'') cout << "\\\'";
		else if (string_[i] == '\"') cout << "\\\"";
		else cout << string_[i];
	}
	cout << "\"";
}

String *String::from_string(const char *expression){
	int len = strlen(expression);
	if (expression[0] != '\"' || expression[len - 1] != '\"') return NULL;
	string str = expression;
	return new String(str.substr(1,len-2));
}

String *String::makeStr(Number *number, Character *ch){
	int n = number->returnInt();
	string res = "";
	for (int i = 0; i < n; ++i){
		res += ch->char_;
	}
	return new String(res);
}
