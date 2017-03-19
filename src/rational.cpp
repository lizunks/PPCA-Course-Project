#include "rational.h"
#include "float.h"
#include "complex.h"
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <complex>
#include <iostream> 
using namespace std; 

int strToInt(string a){
	int len = a.size();
	int b = 0;
	for (int i = 0; i<len; ++i){
		b *= 10;
		b += a[i] - '0';
	}
	return b;
}

Rational::Rational(string numerator, string denominator) :numerator_(numerator), denominator_(denominator){
	type_ = RATIONAL;
	reduce();
}

Rational::~Rational(){}

void Rational::reduce(){
	assert(denominator_ && "denominator is zero");
	if (!numerator_){
		denominator_.number_ = "1";
		return;
	}
	LongInt big, small, tmp;
	big = max(numerator_, denominator_);
	small = min(numerator_, denominator_);
	tmp = big % small;
	while (tmp = big % small){ //辗转相除（负数兼容）
		big = small;
		small = tmp;
	}
	numerator_ = numerator_ / small;
	denominator_ = denominator_ / small;
	if (denominator_ < LongInt("0")){ //分母为负时 分子分母同时取相反数 
		numerator_ = LongInt("0") - numerator_;
		denominator_ = LongInt("0") - denominator_;
	}
}

Number *Rational::convert(Number *number2){ //转换类型 
	assert(number2->type_ <= type_);
	Rational *result = new Rational();
	switch (number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->numerator_ = tmp->numerator_;
			result->denominator_ = tmp->denominator_;
			break;
		}
		default:
			assert(0 && "type_ not defined");
	}
	result->reduce();
	return result;
}

Number *Rational::add(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->numerator_ = numerator_*tmp->denominator_ + denominator_*tmp->numerator_;
	result->denominator_ = denominator_ * tmp->denominator_;
	result->reduce();
	return result;
}

Number *Rational::sub(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->numerator_ = numerator_*tmp->denominator_ - denominator_*tmp->numerator_;
	result->denominator_ = denominator_ * tmp->denominator_;
	result->reduce();
	return result;
}

Number *Rational::mul(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->numerator_ = numerator_ * tmp->numerator_;
	result->denominator_ = denominator_ * tmp->denominator_;
	result->reduce();
	return result;
}

Number *Rational::div(Number *number2) {
	Rational *tmp = SCAST_RATIONAL(number2);
	Rational *result = new Rational();
	result->numerator_ = numerator_ * tmp->denominator_;
	result->denominator_ = denominator_ * tmp->numerator_;
	result->reduce();
	return result;
}

Number *Rational::abs(){
	string num = numerator_.number_, den = denominator_.number_;
	if (num[0] == '-') num.erase(0, 1);
	return new Rational(num, den);
}

Number *Rational::quo(Number *number2){ 
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_.number_ != "1" || tmp->denominator_.number_ != "1") return NULL;
	return new Rational((numerator_ / tmp->numerator_).number_, "1");
}

Number *Rational::rem(Number *number2){ 
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_.number_ != "1" || tmp->denominator_.number_ != "1") return NULL;
	return new Rational((numerator_ % tmp->numerator_).number_, "1");
}

Number *Rational::mod(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_.number_ != "1" || tmp->denominator_.number_ != "1") return NULL;
	if (LongInt("0") < numerator_*tmp->numerator_ || numerator_.number_=="0") return new Rational((numerator_%tmp->numerator_).number_, "1");
	else return new Rational((numerator_%tmp->numerator_ + tmp->numerator_).number_, "1");
}

Number *Rational::gcd(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_.number_ != "1" || tmp->denominator_.number_ != "1") return NULL;
	if (numerator_.number_ == "0") return new Rational(tmp->numerator_.number_, "1");
	else if (tmp->numerator_.number_ == "0") return new Rational(numerator_.number_, "1");
	LongInt big, small, a;
	big = max(numerator_, tmp->numerator_);
	small = min(numerator_, tmp->numerator_);
	while (a = big % small){ 
		big = small;
		small = a;
	}
	string result = small.number_;
	if (result[0] == '-') result.erase(0, 1);
	return new Rational(result, "1");
}

Number *Rational::lcm(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_.number_ != "1" || tmp->denominator_.number_ != "1") return NULL;
	if (numerator_.number_[0] == '0' || tmp->numerator_.number_[0] == '0') return new Rational("0", "1");
	string result = SCAST_RATIONAL(this->mul(tmp)->div(this->gcd(tmp)))->numerator_.number_;
	if (result[0] == '-') result.erase(0, 1);
	return new Rational(result, "1");
}

Number *Rational::expt(Number *number2){
	Float *f = new Float();
	Float *base = SCAST_FLOAT(f->convert(this));
	Number *exp = f->convert(number2);
	Number *result = base->expt(exp);
	delete f;
	return result;
}

Number *Rational::sqt(){
	Float *f = new Float();
	Float *radi = SCAST_FLOAT(f->convert(this));
	Number *result = radi->sqt();
	delete f;
	return result;
}

Number *Rational::num(){
	string num = numerator_.number_;
	return new Rational(num, "1");
}

Number *Rational::den(){
	string den = denominator_.number_;
	return new Rational(den, "1");
}

Number *Rational::flo(){
	if (denominator_.number_ == "1") return new Rational(numerator_.number_, "1");
	else{
		LongInt result = numerator_ / denominator_;
		if (numerator_.number_[0] == '-') return new Rational((result - LongInt("1")).number_, "1");
		else return new Rational(result.number_, "1");
	}
}

Number *Rational::cei(){
	if (denominator_.number_ == "1") return new Rational(numerator_.number_, "1");
	else{
		LongInt result = numerator_ / denominator_;
		if (numerator_.number_[0] == '-') return new Rational(result.number_, "1");
		else return new Rational((result + LongInt("1")).number_, "1");
	}
}

Number *Rational::tru(){
	if (denominator_.number_ == "1") return new Rational(numerator_.number_, "1");
	else{
		LongInt result = numerator_ / denominator_;
		return new Rational(result.number_, "1");
	}
}

Number *Rational::rou(){
	if (denominator_.number_ == "1") return new Rational(numerator_.number_, "1");
	else{
		if (numerator_.number_[0] != '-'){ //正数
			Rational *remainder = new Rational((numerator_%denominator_).number_, denominator_.number_);
			Rational *half = new Rational("1", "2");
			Rational *judge = SCAST_RATIONAL(remainder->sub(half));
			if (judge->numerator_.number_[0] == '0'){ //余1/2 
				delete remainder, half, judge;
				if ((numerator_ / denominator_%LongInt("2")).number_ == "0") return tru(); //5靠偶
				else return cei();
			}
			else if (judge->numerator_.number_[0] == '-'){ //四舍
				delete remainder, half, judge;
				return flo();
			}
			else{											//六入
				delete remainder, half, judge;
				return cei();
			}
		}
		else{										//负数
			Rational *remainder = new Rational((numerator_%denominator_).number_, denominator_.number_);
			Rational *half = new Rational("-1", "2");
			Rational *judge = SCAST_RATIONAL(remainder->sub(half));
			if (judge->numerator_.number_[0] == '0'){ //余-1/2 
				delete remainder, half, judge;
				if ((numerator_ / denominator_%LongInt("2")).number_ == "0") return tru(); //5靠偶
				else return flo();
			}
			else if (judge->numerator_.number_[0] == '-'){ //四舍
				delete remainder, half, judge;
				return flo();
			}
			else{											//六入
				delete remainder, half, judge;
				return cei();
			}
		}
	}
}

Number *Rational::toexact(){
	return new Rational(numerator_.number_, denominator_.number_);
}

Number *Rational::toinexact(){
	Float *tmp = new Float();
	Number *result = tmp->convert(this);
	delete tmp;
	return result;
}

Number *Rational::maxi(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (numerator_*tmp->denominator_ < denominator_*tmp->numerator_) return new Rational(tmp->numerator_.number_, tmp->denominator_.number_);
	else return new Rational(numerator_.number_, denominator_.number_);
}

Number *Rational::mini(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (numerator_*tmp->denominator_ < denominator_*tmp->numerator_) return new Rational(numerator_.number_, denominator_.number_);
	else return new Rational(tmp->numerator_.number_, tmp->denominator_.number_);
}

Number *Rational::sinx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->sinx();
	delete f;
	return result;
}

Number *Rational::cosx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->cosx();
	delete f;
	return result;
}

Number *Rational::tanx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->tanx();
	delete f;
	return result;
}

Number *Rational::asinx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->asinx();
	delete f;
	return result;
}

Number *Rational::acosx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->acosx();
	delete f;
	return result;
}

Number *Rational::atanx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->atanx();
	delete f;
	return result;
}

Number *Rational::atanxy(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	Float *f = new Float();
	Float *tmpx = SCAST_FLOAT(f->convert(this));
	Float *tmpy = SCAST_FLOAT(f->convert(tmp));
	Number *result = tmpx->atanxy(tmpy);
	delete f, tmpx, tmpy;
	return result;
}

Number *Rational::expx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->expx();
	delete f;
	return result;
}

Number *Rational::logx(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->logx();
	delete f;
	return result;
}

Number *Rational::makerectangular(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	return new Complex(this, tmp);
}

Number *Rational::makepolar(Number *number2){
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

Number *Rational::realpart(){
	return new Rational(numerator_.number_,denominator_.number_);
}

Number *Rational::imagpart(){
	return new Rational("0", "1");
}

Number *Rational::mag(){
	string num = numerator_.number_, den = denominator_.number_;
	if (num[0] == '-') num.erase(0, 1);
	return new Rational(num, den);
}

Number *Rational::ang(){
	Float *f = new Float();
	Float *tmp = SCAST_FLOAT(f->convert(this));
	Number *result = tmp->ang();
	delete f;
	return result;
}

Boolean *Rational::less(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (numerator_*tmp->denominator_ < denominator_*tmp->numerator_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::lessequal(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (numerator_*tmp->denominator_ < denominator_*tmp->numerator_ || numerator_*tmp->denominator_ == denominator_*tmp->numerator_) return new Boolean(true);
	else return new Boolean(false);
}


Boolean *Rational::greater(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_*tmp->numerator_ < numerator_*tmp->denominator_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::greaterequal(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_*tmp->numerator_ < numerator_*tmp->denominator_ || denominator_*tmp->numerator_ == numerator_*tmp->denominator_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isEqual(Number *number2){
	Rational *tmp = SCAST_RATIONAL(number2);
	if (denominator_*tmp->numerator_ == numerator_*tmp->denominator_) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isZero(){
	if (numerator_.number_ == "0") return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isNegative(){
	if (numerator_.number_[0] == '-') return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isPositive(){
	if (numerator_.number_[0] != '-' && numerator_.number_[0]!='0') return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isOdd(){
	if (denominator_.number_ != "1") return NULL;
	if (numerator_%LongInt("2") == LongInt("1") || numerator_%LongInt("2") == LongInt("-1")) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isEven(){
	if (denominator_.number_ != "1") return NULL;
	if (numerator_%LongInt("2") == LongInt("0")) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isInteger(){
	if (denominator_.number_ == "1") return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Rational::isRational(){
	return new Boolean(true);
}

Boolean *Rational::isReal(){
	return new Boolean(true);
}

Boolean *Rational::isComplex(){
	return new Boolean(true);
}

Boolean *Rational::isNumber(){
	return new Boolean(true);
}

Boolean *Rational::isChar(){
	return new Boolean(false);
}

Boolean *Rational::isString(){
	return new Boolean(false);
}

Boolean *Rational::isExact(){
	return new Boolean(true);
}

Boolean *Rational::isInexact(){
	return new Boolean(false);
}

Character *Rational::intToChar(){
	if (denominator_.number_ != "1") return NULL;
	return new Character(strToInt(numerator_.number_));
}

String *Rational::numToStr(){
	string res = "";
	for (int i = 0; i < numerator_.number_.size(); ++i){
		res += numerator_.number_[i];
	}
	if (denominator_ != LongInt("1")){
		res += '/';
		for (int i = 0; i < denominator_.number_.size(); ++i){
			res += denominator_.number_[i];
		}
	}
	return new String(res);
}

int Rational::returnInt(){
	assert(denominator_.number_ == "1" && "Integer Expected!");
	return int(double(numerator_));
}

void Rational::print(){
	numerator_.print();
	if (denominator_ != LongInt("1")){
		printf("/");
		denominator_.print();
	}
}

Rational *Rational::from_string(const char *expression_in_char){ //把string(char)的一个单元变成rational 
	string expression = expression_in_char;
	int separate_pos = expression.find("/");
	if (separate_pos != expression.npos){ //'/'存在 
		string num = expression.substr(0, separate_pos);
		if (num == "") return NULL;
		bool flag_negative = false;
		if (num[0] == '+') num.erase(0, 1);
		else if (num[0] == '-'){
			flag_negative = true;
			num = num.erase(0, 1);
		}
		for (int i = 0; i<num.size(); ++i){ //存在非法字符 
			if (num[i]<'0' || num[i]>'9'){
				return NULL; break;
			}
		}
		int count = 0;
		for (int i = 0; i<num.size() - 1; ++i){
			if (num[i] == '0') ++count;
			else break;
		}
		string numerator = num.erase(0, count);
		if (flag_negative&&numerator != "0") numerator = "-" + numerator;

		string den = expression.erase(0, separate_pos + 1);
		if (den == "") return NULL;
		bool flag_negative2 = false;
		if (den[0] == '+') num.erase(0, 1);
		else if (den[0] == '-'){
			flag_negative2 = true;
			den = den.erase(0, 1);
		}
		for (int i = 0; i<den.size(); ++i){ //存在非法字符 
			if (den[i]<'0' || den[i]>'9'){
				return NULL; break;
			}
		}
		int count2 = 0;
		for (int i = 0; i<den.size() - 1; ++i){
			if (den[i] == '0') ++count2;
			else break;
		}
		string denominator = den.erase(0, count2);
		if (flag_negative2&&numerator != "0") denominator = "-" + denominator;

		return new Rational(numerator, denominator);
	}
	else{
		string num = expression;
		if (num == "") return NULL;
		bool flag_negative = false;
		if (num[0] == '+') num.erase(0, 1);
		else if (num[0] == '-'){
			flag_negative = true;
			num = num.erase(0, 1);
		}
		for (int i = 0; i<num.size(); ++i){ //存在非法字符 
			if (num[i]<'0' || num[i]>'9'){
				return NULL; break;
			}
		}
		int count = 0;
		for (int i = 0; i<num.size() - 1; ++i){
			if (num[i] == '0') ++count;
			else break;
		}
		string numerator = num.erase(0, count);
		if (flag_negative&&numerator != "0") numerator = "-" + numerator;
		return new Rational(numerator, "1");
	}
}

//#include <iostream>
//int main(){
//	while (true){
//		string x, y;
//		cin >> x >> y;
//		Rational *a=new Rational(x,y);
//		a->print();
//		string m, n;
//		cin >> m >> n;
//		Rational *b=new Rational(m,n);
//		b->print();
//		(a->add(b))->print();
//	}
//	return 0;
//} 

