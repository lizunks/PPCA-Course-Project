#include "float.h"
#include "rational.h"
#include "complex.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <complex>
#include <iostream>
using namespace std;

#define ABS(x) ((x)<0?(-(x)):(x))

void turn(string &str, double n){
	if (n<10) str += n + '0';
	else{
		turn(str, n / 10);
		str += fmod(n, 10) + '0';
	}
}

string doubleToString(double d){ //only for double which is integer
	string str="";
	if (d < 0){
		d = fabs(d);
		str += '-';
	}
	turn(str, d);
	return str;

	//ostringstream os;
	//os.precision(18);
	//if (os << d)
	//	return os.str();
	//return "invalid conversion";
}

string doubleToStr(double d){
	ostringstream os;
	os.precision(18);
	os << d;
	string ans = os.str();
	if (trunc(d) == d && fabs(d) < 1e18) ans += ".0";
	return ans;
}

double stringToDouble(const string &str){
	istringstream is(str);
	double num;
	is >> num;
	return num;
}


Float::Float(double number) : number_(number){
	type_ = FLOAT;
}

Float::~Float(){}

Number *Float::convert(Number *number2){
	assert(number2->type_ <= type_);
	Float *result = new Float();
	switch(number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->number_ = (double)tmp->numerator_ / (double)tmp->denominator_;
			// printf("%lf", result->number_);
			break;
		}
		case FLOAT:{
			Float *tmp = SCAST_FLOAT(number2);
			result->number_ = tmp->number_;
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	return result;
}

Number *Float::add(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ + tmp->number_);
	return result;
}

Number *Float::sub(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ - tmp->number_);
	return result;
}

Number *Float::mul(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	Float *result = new Float(number_ * tmp->number_);
	return result;
}

Number *Float::div(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	assert(ABS(tmp->number_)!=0 && "divide zero");
	Float *result = new Float(number_ / tmp->number_);
	return result;
}

Number *Float::abs(){
	return new Float (fabs(number_));
}

Number *Float::quo(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ != trunc(number_) || tmp->number_ != trunc(tmp->number_)) return NULL;
	return new Float(trunc(number_ / tmp->number_));
}

Number *Float::rem(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ != trunc(number_) || tmp->number_ != trunc(tmp->number_)) return NULL;
	return new Float(fmod(number_, tmp->number_));
}

Number *Float::mod(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ != trunc(number_) || tmp->number_ != trunc(tmp->number_)) return NULL;
	if (number_*tmp->number_ >= 0) return new Float(fmod(number_, tmp->number_));
	else return new Float(fmod(number_, tmp->number_) + tmp->number_);
}

Number *Float::gcd(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ != trunc(number_) || tmp->number_ != trunc(tmp->number_)) return NULL;
	if (number_ == 0) return new Float(tmp->number_);
	else if (tmp->number_ == 0) return new Float(number_);
	double big, small, a;
	big = max(number_, tmp->number_);
	small = min(number_, tmp->number_);
	while (a = fmod(big, small)){
		big = small;
		small = a;
	}
	return new Float(fabs(small));
}

Number *Float::lcm(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ != trunc(number_) || tmp->number_ != trunc(tmp->number_)) return NULL;
	if (number_ == 0 || tmp->number_ == 0) return new Float(0);
	double result = SCAST_FLOAT(this->mul(tmp)->div(this->gcd(tmp)))->number_;
	return new Float(fabs(result));

}

Number *Float::expt(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	double ans = pow(number_, tmp->number_);
	if (ans != ans){ //special cases like -1 1/2
		Complex *c = new Complex();
		Complex *c1 = SCAST_COMPLEX(c->convert(this));
		Complex *c2 = SCAST_COMPLEX(c->convert(tmp));
		Complex *result = SCAST_COMPLEX(c1->expt(c2));
		delete c, c1, c2;
		return result;
	}
	else return new Float(ans);
}

Number *Float::sqt(){
	double ans = sqrt(number_);
	if (ans != ans){ //special cases like -1
		Complex *c = new Complex();
		Complex *c1 = SCAST_COMPLEX(c->convert(this));
		Complex *result = SCAST_COMPLEX(c1->sqt());
		delete c, c1;
		return result;
	}
	else return new Float(ans);
}

Number *Float::num(){
	Rational *tmp = SCAST_RATIONAL(this->toexact());
	string num = tmp->numerator_.number_;
	delete tmp;
	return new Float(stringToDouble(num));
}

Number *Float::den(){
	Rational *tmp = SCAST_RATIONAL(this->toexact());
	string den = tmp->denominator_.number_;
	delete tmp;
	return new Float(stringToDouble(den));
}

Number *Float::flo(){
	return new Float(floor(number_));
}

Number *Float::cei(){
	return new Float(ceil(number_));
}

Number *Float::tru(){
	return new Float(trunc(number_));
}

Number *Float::rou(){
	if (fabs(number_ - trunc(number_)) == 0.5){
		if (fmod(trunc(number_), 2) == 0) return new Float(trunc(number_));
		else return new Float(round(number_));
	}
	else return new Float(round(number_));

}

Number *Float::toexact(){ //·ÂÕÕRacket²»¶Ï³Ë2
	double num = number_;
	LongInt den("1");
	while (num != trunc(num)){
		num *= 2;
		den = den * LongInt("2");
	}
	while (fmod(num, 2) == 0 && den.number_ != "1"){
		num /= 2;
		den = den / LongInt("2");
	}
	return new Rational(doubleToString(num), den.number_);
}

Number *Float::toinexact(){
	return new Float(number_);
}

Number *Float::maxi(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	return new Float(max(number_,tmp->number_));
}

Number *Float::mini(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	return new Float(min(number_, tmp->number_));
}

Number *Float::sinx(){
	return new Float(sin(number_));
}

Number *Float::cosx(){
	return new Float(cos(number_));
}

Number *Float::tanx(){
	return new Float(tan(number_));
}

Number *Float::asinx(){
	double ans = asin(number_);
	if (ans != ans){
		Complex *c = new Complex();
		Complex *c1 = SCAST_COMPLEX(c->convert(this));
		Complex *result = SCAST_COMPLEX(c1->asinx());
		delete c, c1;
		return result;
	}
	else return new Float(ans);
}

Number *Float::acosx(){
	double ans = acos(number_);
	if (ans != ans){
		Complex *c = new Complex();
		Complex *c1 = SCAST_COMPLEX(c->convert(this));
		Complex *result = SCAST_COMPLEX(c1->acosx());
		delete c, c1;
		return result;
	}
	else return new Float(ans);
}

Number *Float::atanx(){
	return new Float(atan(number_));
}

Number *Float::atanxy(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	return new Float(atan(number_/tmp->number_));
}

Number *Float::expx(){
	return new Float(exp(number_));
}

Number *Float::logx(){
	assert(number_ != 0 && "log0 is not defined");
	double ans = log(number_);
	if (ans != ans){
		Complex *c = new Complex();
		Complex *c1 = SCAST_COMPLEX(c->convert(this));
		Complex *result = SCAST_COMPLEX(c1->logx());
		delete c, c1;
		return result;
	}
	else return new Float(ans);
}

Number *Float::makerectangular(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	return new Complex(this, tmp);
}

Number *Float::makepolar(Number *number2){
	Float *f = new Float();
	Float *mag = SCAST_FLOAT(f->convert(this));
	Float *ang = SCAST_FLOAT(f->convert(number2));
	complex<double> res = polar(mag->number_, ang->number_);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, mag, ang, real, imag;
	return result;
}

Number *Float::realpart(){
	return new Float(number_);
}

Number *Float::imagpart(){
	return new Float(0);
}

Number *Float::mag(){
	return new Float(fabs(number_));
}

Number *Float::ang(){
	Complex *c = new Complex();
	Float *tmp = SCAST_FLOAT(c->convert(this));
	Number *result = tmp->ang();
	delete c;
	return result;
}

Boolean *Float::less(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ < tmp->number_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::lessequal(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ < tmp->number_ || number_ == tmp->number_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::greater(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ > tmp->number_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::greaterequal(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ > tmp->number_ || number_ == tmp->number_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isEqual(Number *number2){
	Float *tmp = SCAST_FLOAT(number2);
	if (number_ == tmp->number_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isZero(){
	if (number_ == 0) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isNegative(){
	if (number_ < 0) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isPositive(){
	if (number_ > 0) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isOdd(){
	if (number_ != trunc(number_)) return NULL;
	if (fmod(number_, 2) == 1 || fmod(number_, 2) == -1) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isEven(){
	if (number_ != trunc(number_)) return NULL;
	if (fmod(number_, 2) == 0) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isInteger(){
	if (number_ == trunc(number_)) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Float::isRational(){
	return new Boolean(true);
}

Boolean *Float::isReal(){
	return new Boolean(true);
}

Boolean *Float::isComplex(){
	return new Boolean(true);
}

Boolean *Float::isNumber(){
	return new Boolean(true);
}

Boolean *Float::isChar(){
	return new Boolean(false);
}

Boolean *Float::isString(){
	return new Boolean(false);
}

Boolean *Float::isExact(){
	return new Boolean(false);
}

Boolean *Float::isInexact(){
	return new Boolean(true);
}

Character *Float::intToChar(){
	if (number_ != trunc(number_)) return NULL;
	return new Character(int(number_));
}

String *Float::numToStr(){
	return new String(doubleToStr(number_));
}

int Float::returnInt(){
	assert(trunc(number_)==number_ && "Integer Expected!");
	return int(number_);
}

void Float::print(){
	if (trunc(number_) == number_ && fabs(number_)< 1e18){ printf("%.18g", number_); cout << ".0"; }
	else printf("%.18g", number_);
}

Float *Float::from_string(const char *expression){
    char *end_pointer;
    double val = strtod(expression, &end_pointer);
    if (end_pointer == expression || end_pointer != expression + strlen(expression))
	    return NULL;
    return new Float(val);
}


// int main(){
// 	Float *a = new Float(1.2);
// 	Float *b = new Float(1.3);
// 	(a->add(b))->print();
// }
