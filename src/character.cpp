#include "character.h"
#include "rational.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <cctype>
using namespace std;

char upper(char ch){
	if (ch >= 'a' && ch <= 'z') return (char)(ch - 32);
	else return ch;
}

char lower(char ch){
	if (ch >= 'A' && ch <= 'Z') return (char)(ch + 32);
	else return ch;
}

void intToStr(string &str, int n){
	if (n / 10 == 0) str += n + '0';
	else{
		intToStr(str, n / 10);
		str += n % 10 + '0';
	}
}

Character::Character(char ch) :char_(ch){
	type_ = CHARACTER;
}

Character::~Character(){}

Boolean *Character::charIsEqual(Character *char2){
	return new Boolean(char_ == char2->char_);
}

Boolean *Character::charIsCiEqual(Character *char2){
	return new Boolean(upper(char_) == upper(char2->char_));
}

Boolean *Character::charLess(Character *char2){
	return new Boolean(char_ < char2->char_);
}

Boolean *Character::charCiLess(Character *char2){
	return new Boolean(upper(char_) < upper(char2->char_));
}

Boolean *Character::charLessequal(Character *char2){
	return new Boolean(char_ <= char2->char_);
}

Boolean *Character::charCiLessequal(Character *char2){
	return new Boolean(upper(char_) <= upper(char2->char_));
}

Boolean *Character::charIsAlpha(){
	return new Boolean((char_ >= 'a' && char_ <= 'z') || (char_ >= 'A' && char_ <= 'Z'));
}

Boolean *Character::charIsNum(){
	return new Boolean(char_ >= '0' && char_ <= '9');
}

Boolean *Character::charIsSpace(){
	if (isspace(char_) == 0) return new Boolean(false);
	else return new Boolean(true);
}

Boolean *Character::charIsUpCase(){
	return new Boolean(char_ >= 'A' && char_ <= 'Z');
}

Boolean *Character::charIsLowCase(){
	return new Boolean(char_ >= 'a' && char_ <= 'z');
}


Character *Character::charToUpCase(){
	if (char_ >= 'a' && char_ <= 'z') return new Character(char_ - 32);
	else return new Character(char_);
}

Character *Character::charToDownCase(){
	if (char_ >= 'A' && char_ <= 'Z') return new Character(char_ + 32);
	else return new Character(char_);
}

Datatype *Character::charToInt(){
	string tmp = "";
	intToStr(tmp, int(char_));
	return new Rational(tmp, "1");
}


Boolean *Character::isInteger(){
	return new Boolean(false);
}

Boolean *Character::isRational(){
	return new Boolean(false);
}

Boolean *Character::isReal(){
	return new Boolean(false);
}

Boolean *Character::isComplex(){
	return new Boolean(false);
}

Boolean *Character::isNumber(){
	return new Boolean(false);
}

Boolean *Character::isChar(){
	return new Boolean(true);
}

Boolean *Character::isString(){
	return new Boolean(false);
}

void Character::print(){
	if (char_ == '\b') printf("#\\backspace");
	else if (char_ == '\f') printf("#\\page");
	else if (char_ == '\n') printf("#\\newline");
	else if (char_ == '\r') printf("#\\return");
	else if (char_ == '\t') printf("#\\tab");
	else if (char_ == '\v') printf("#\\vtab");
	else if (char_ == ' ') printf("#\\space");
	else printf("#\\%c", char_);
}

Character *Character::from_string(const char *expression){
	if (strlen(expression) != 3) return NULL; 
	if (!(expression[0] == '#' && expression[1] == '\\')) return NULL;
	return new Character(expression[2]);
}
