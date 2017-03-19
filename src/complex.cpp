#include "rational.h"
#include "float.h"
#include "complex.h"
#include <cmath>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <string>
#include <complex>
#include <iostream>
using namespace std;

Complex::Complex(string real, string imag) {
	type_ = COMPLEX;
	const char* real2(real.c_str());
	const char* imag2(imag.c_str());
	Float *f = new Float();
	real_ = Rational::from_string(real2);
	imag_ = Rational::from_string(imag2);
	if (!real_ || !imag_){
		if (!imag_) imag_ = Float::from_string(imag2);
		else imag_ = f->convert(imag_);
		if (!real_) real_ = Float::from_string(real2);
		else real_ = f->convert(real_);
		if (!real_ || !imag_) throw 0;
	}
}

Complex::Complex(Number *real, Number *imag){
	type_ = COMPLEX;
	switch (real->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(real);
			real_ = new Rational(tmp->numerator_.number_,tmp->denominator_.number_);
			break;
		}
		case FLOAT:{
			Float *tmp = SCAST_FLOAT(real);
			real_ = new Float(tmp->number_);
			break;
		}
		default:{
			assert(0 && "type_ not defined");
		}
	}
	switch (imag->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(imag);
			imag_ = new Rational(tmp->numerator_.number_,tmp->denominator_.number_);
			break;
		}
		case FLOAT:{
			Float *tmp = SCAST_FLOAT(imag);
			imag_ = new Float(tmp->number_);
			break;
		}
		default:{
			assert(0 && "type_ not defined");
		}
	}
}

Complex::~Complex(){
	delete real_, imag_;
}

Number *Complex::convert(Number *number2){
	assert(number2->type_ <= type_);
	Complex *result = new Complex();
	switch (number2->type_){
		case RATIONAL:{
			Rational *tmp = SCAST_RATIONAL(number2);
			result->real_ = new Rational(tmp->numerator_.number_,tmp->denominator_.number_);
			result->imag_ = new Rational();
			break;
		}
		case FLOAT:{
			Float *tmp = SCAST_FLOAT(number2);
			result->real_ = new Float(tmp->number_);
			result->imag_ = new Float();
			break;
		}
		case COMPLEX:{
			Complex *tmp = SCAST_COMPLEX(number2);
			if (tmp->real_->type_ == FLOAT){
				Float *real = SCAST_FLOAT(tmp->real_);
				Float *imag = SCAST_FLOAT(tmp->imag_);
				result->real_ = new Float(real->number_);
				result->imag_ = new Float(imag->number_);
			}
			else{
				Rational *real = SCAST_RATIONAL(tmp->real_);
				Rational *imag = SCAST_RATIONAL(tmp->imag_);
				result->real_ = new Rational(real->numerator_.number_,real->denominator_.number_);
				result->imag_ = new Rational(imag->numerator_.number_,imag->denominator_.number_);
			}
			break;
		}
		default:{
			assert(0 && "type_ not defined");
		}
	}
	return result;
}

Number *Complex::add(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();
	if (real_->type_ == tmp->real_->type_){
		result->real_ = real_->add(tmp->real_);
		result->imag_ = imag_->add(tmp->imag_);
	}
	else if (real_->type_ == RATIONAL&&tmp->real_->type_ == FLOAT){
		Number *tmpreal_ = tmp->real_->convert(real_);
		Number *tmpimag_ = tmp->imag_->convert(imag_);
		result->real_ = tmpreal_->add(tmp->real_);
		result->imag_ = tmpimag_->add(tmp->imag_);
		delete tmpreal_, tmpimag_;
	}
	else{
		Number *tmpreal_ = real_->convert(tmp->real_);
		Number *tmpimag_ = imag_->convert(tmp->imag_);
		result->real_ = real_->add(tmpreal_);
		result->imag_ = imag_->add(tmpimag_);
		delete tmpreal_, tmpimag_;
	}
	return result;
}

Number *Complex::sub(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();
	if (real_->type_ == tmp->real_->type_){
		result->real_ = real_->sub(tmp->real_);
		result->imag_ = imag_->sub(tmp->imag_);
	}
	else if (real_->type_ == RATIONAL&&tmp->real_->type_ == FLOAT){
		Number *tmpreal_ = tmp->real_->convert(real_);
		Number *tmpimag_ = tmp->imag_->convert(imag_);
		result->real_ = tmpreal_->sub(tmp->real_);
		result->imag_ = tmpimag_->sub(tmp->imag_);
		delete tmpreal_, tmpimag_;
	}
	else{
		Number *tmpreal_ = real_->convert(tmp->real_);
		Number *tmpimag_ = imag_->convert(tmp->imag_);
		result->real_ = real_->sub(tmpreal_);
		result->imag_ = imag_->sub(tmpimag_);
		delete tmpreal_, tmpimag_;
	}
	return result;
}

Number *Complex::mul(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();
	if (real_->type_ == tmp->real_->type_){
		result->real_ = (real_->mul(tmp->real_))->sub(imag_->mul(tmp->imag_));
		result->imag_ = (real_->mul(tmp->imag_))->add(imag_->mul(tmp->real_));
	}
	else if (real_->type_ == RATIONAL&&tmp->real_->type_ == FLOAT){
		Number *tmpreal_ = tmp->real_->convert(real_);
		Number *tmpimag_ = tmp->imag_->convert(imag_);
		result->real_ = (tmpreal_->mul(tmp->real_))->sub(tmpimag_->mul(tmp->imag_));
		result->imag_ = (tmpreal_->mul(tmp->imag_))->add(tmpimag_->mul(tmp->real_));
		delete tmpreal_, tmpimag_;
	}
	else{
		Number *tmpreal_ = real_->convert(tmp->real_);
		Number *tmpimag_ = imag_->convert(tmp->imag_);
		result->real_ = (real_->mul(tmpreal_))->sub(imag_->mul(tmpimag_));
		result->imag_ = (real_->mul(tmpimag_))->add(imag_->mul(tmpreal_));
		delete tmpreal_, tmpimag_;
	}
	return result;
}

Number *Complex::div(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Complex *result = new Complex();
	if (real_->type_ == tmp->real_->type_){
		result->real_ = ((real_->mul(tmp->real_))->add((imag_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
		result->imag_ = ((imag_->mul(tmp->real_))->sub((real_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
	}
	else if (real_->type_ == RATIONAL&&tmp->real_->type_ == FLOAT){
		Number *tmpreal_ = tmp->real_->convert(real_);
		Number *tmpimag_ = tmp->imag_->convert(imag_);
		result->real_ = ((tmpreal_->mul(tmp->real_))->add((tmpimag_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
		result->imag_ = ((tmpimag_->mul(tmp->real_))->sub((tmpreal_->mul(tmp->imag_))))->div(((tmp->real_)->mul(tmp->real_))->add((tmp->imag_)->mul(tmp->imag_)));
		delete tmpreal_, tmpimag_;
	}
	else{
		Number *tmpreal_ = real_->convert(tmp->real_);
		Number *tmpimag_ = imag_->convert(tmp->imag_);
		result->real_ = ((real_->mul(tmpreal_))->add((imag_->mul(tmpimag_))))->div(((tmpreal_)->mul(tmpreal_))->add((tmpimag_)->mul(tmpimag_)));
		result->imag_ = ((imag_->mul(tmpreal_))->sub((real_->mul(tmpimag_))))->div(((tmpreal_)->mul(tmpreal_))->add((tmpimag_)->mul(tmpimag_)));
		delete tmpreal_, tmpimag_;
	}
	return result;
}

Number *Complex::abs(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->abs();
}

Number *Complex::quo(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ !=0 ) return NULL;
	}
	return real_->quo(tmp->real_);
}

Number *Complex::rem(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ !=0 ) return NULL;
	}
	return real_->rem(tmp->real_);
}

Number *Complex::mod(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ !=0) return NULL;
	}
	return real_->mod(tmp->real_);
}

Number *Complex::gcd(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ != 0) return NULL;
	}
	return real_->gcd(tmp->real_);
}

Number *Complex::lcm(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ != 0) return NULL;
	}
	return real_->lcm(tmp->real_);
}

Number *Complex::expt(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	Float *f = new Float();
	Float *real1 =SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	Float *real2 = SCAST_FLOAT(f->convert(tmp->real_));
	Float *imag2 = SCAST_FLOAT(f->convert(tmp->imag_));
	complex<double> a(real1->number_, imag1->number_), b(real2->number_, imag2->number_);
	complex<double> res = pow(a, b);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real2, imag2, real, imag;
	return result;
}

Number *Complex::sqt(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = sqrt(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
}

Number *Complex::num(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->num();
}

Number *Complex::den(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->den();
}

Number *Complex::flo(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->flo();
}

Number *Complex::cei(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->cei();
}

Number *Complex::tru(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->tru();
}

Number *Complex::rou(){
	if (real_->type_ == RATIONAL){
		Rational *imag = SCAST_RATIONAL(imag_);
		if (imag->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag = SCAST_FLOAT(imag_);
		if (imag->number_ != 0) return NULL;
	}
	return real_->rou();
}

Number *Complex::toexact(){
	Number *real = real_->toexact(), *imag = imag_->toexact();
	return new Complex(real, imag);
}

Number *Complex::toinexact(){
	Number *real = real_->toinexact(), *imag = imag_->toinexact();
	return new Complex(real, imag);
}

Number *Complex::maxi(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ !=0) return NULL;
	}
	return real_->maxi(tmp->real_);
}

Number *Complex::mini(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	if (real_->type_ == RATIONAL){
		Rational *imag1 = SCAST_RATIONAL(imag_), *imag2 = SCAST_RATIONAL(tmp->imag_);
		if (imag1->numerator_.number_ != "0" || imag2->numerator_.number_ != "0") return NULL;
	}
	else{
		Float *imag1 = SCAST_FLOAT(imag_), *imag2 = SCAST_FLOAT(tmp->imag_);
		if (imag1->number_ != 0 || imag2->number_ !=0) return NULL;
	}
	return real_->mini(tmp->real_);
}

Number *Complex::sinx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = sin(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
}

Number *Complex::cosx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = cos(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
}

Number *Complex::tanx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = tan(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
}

Number *Complex::asinx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = asin(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
//	complex<double> i(0, 1), one(1, 0);
//	complex<double> res = (-i)*log(i*a + sqrt(one - pow(a, 2)));
//	Float *real = new Float(res.real()), *imag = new Float(res.imag());
//	if (real->number_ == -0) real->number_ = 0;
//	Complex *result = new Complex(real, imag);
//	delete f, real1, imag1, real, imag;
//	return result;
}

Number *Complex::acosx(){
    Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = acos(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
//	Complex *arcsin = SCAST_COMPLEX(this->asinx());
//	Complex *halfpi = new Complex("1.57079632679489661923", "0");
//	Complex *result = SCAST_COMPLEX(halfpi->sub(arcsin));
//	delete arcsin, halfpi;
//	return result;
}

Number *Complex::atanx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
    complex<double> res = atan(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
//	complex<double> i(0,1), one(1,0), two(2,0);
//	complex<double> res = (log(one + i*a) - log(one - i*a)) / (two*i);
//	Float *real = new Float(res.real()), *imag = new Float(res.imag());
//	if (real->number_ == -0) real->number_ = 0;
//	Complex *result = new Complex(real, imag);
//	delete f, real1, imag1, real, imag;
//	return result;
}

Number *Complex::atanxy(Number *number2){
	return NULL;
}

Number *Complex::expx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = exp(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
}

Number *Complex::logx(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	complex<double> res = log(a);
	Float *real = new Float(res.real()), *imag = new Float(res.imag());
	if (real->number_ == -0) real->number_ = 0;
	Complex *result = new Complex(real, imag);
	delete f, real1, imag1, real, imag;
	return result;
}

Number *Complex::makerectangular(Number *number2){
	return NULL;
}

Number *Complex::makepolar(Number *number2){
	return NULL;
}

Number *Complex::realpart(){
	if (real_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(real_);
		return new Rational(tmp->numerator_.number_, tmp->denominator_.number_);
	}
	else{
		Float *tmp = SCAST_FLOAT(real_);
		return new Float(tmp->number_);
	}
}

Number *Complex::imagpart(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		return new Rational(tmp->numerator_.number_, tmp->denominator_.number_);
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		return new Float(tmp->number_);
	}
}

Number *Complex::mag(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	Float *result = new Float(std::abs(a));
	delete f, real1, imag1;
	return result;
}

Number *Complex::ang(){
	Float *f = new Float();
	Float *real1 = SCAST_FLOAT(f->convert(real_));
	Float *imag1 = SCAST_FLOAT(f->convert(imag_));
	complex<double> a(real1->number_, imag1->number_);
	Float *result = new Float(arg(a));
	delete f, real1, imag1;
	return result;
}

Boolean *Complex::less(Number *number2){
	return NULL;
}

Boolean *Complex::lessequal(Number *number2){
	return NULL;
}

Boolean *Complex::greater(Number *number2){
	return NULL;
}

Boolean *Complex::greaterequal(Number *number2){
	return NULL;
}

Boolean *Complex::isEqual(Number *number2){
	Complex *tmp = SCAST_COMPLEX(number2);
	bool flag1, flag2;
	if (real_->type_ == tmp->real_->type_){
		flag1 = real_->isEqual(tmp->real_)->value_;
		flag2 = imag_->isEqual(tmp->imag_)->value_;
	}
	else if (real_->type_ > tmp->real_->type_){
		Number *conv1, *conv2;
		flag1 = real_->isEqual(conv1 = real_->convert(tmp->real_))->value_;
		flag2 = imag_->isEqual(conv2 = imag_->convert(tmp->imag_))->value_;
		delete conv1, conv2;
	}
	else{
		Number *conv1, *conv2;
		flag1 = (conv1 = tmp->real_->convert(real_))->isEqual(tmp->real_)->value_;
		flag2 = (conv1 = tmp->imag_->convert(imag_))->isEqual(tmp->imag_)->value_;
		delete conv1, conv2;
	}
	if (flag1 && flag2) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Complex::isZero(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return real_->isZero();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return real_->isZero();
	}
	return NULL;
}

Boolean *Complex::isNegative(){
	return NULL;
}

Boolean *Complex::isPositive(){
	return NULL;
}

Boolean *Complex::isOdd(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return real_->isOdd();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return real_->isOdd();
	}
	return NULL;
}

Boolean *Complex::isEven(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return real_->isEven();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return real_->isEven();
	}
	return NULL;
}

Boolean *Complex::isInteger(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return real_->isInteger();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return real_->isInteger();
	}
	return new Boolean(false);
}

Boolean *Complex::isRational(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return real_->isRational();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return real_->isRational();
	}
	return new Boolean(false);
}

Boolean *Complex::isReal(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return real_->isReal();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return real_->isReal();
	}
	return new Boolean(false);
}

Boolean *Complex::isComplex(){
	return new Boolean(true);
}

Boolean *Complex::isNumber(){
	return new Boolean(true);
}

Boolean *Complex::isChar(){
	return new Boolean(false);
}

Boolean *Complex::isString(){
	return new Boolean(false);
}

Boolean *Complex::isExact(){
	if (real_->type_ == RATIONAL) return new Boolean(true);
	else return new Boolean(false);
}

Boolean *Complex::isInexact(){
	if (real_->type_ == FLOAT) return new Boolean(true);
	else return new Boolean(false);
}

Character *Complex::intToChar(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		if (tmp->numerator_.number_ == "0") return SCAST_CHARACTER(real_->intToChar());
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		if (tmp->number_ == 0) return SCAST_CHARACTER(real_->intToChar());
	}
	return NULL;
}

String *Complex::numToStr(){
	string res = "";
	string real = SCAST_STRING(real_->numToStr())->string_, imag = SCAST_STRING(imag_->numToStr())->string_;
	res += real;
	if (imag[0] != '-') res += '+';
	res += imag;
	res += 'i';
	return new String(res);
}

int Complex::returnInt(){
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		assert(tmp->numerator_.number_ == "0" && "Integer Expected!");
		return real_->returnInt();
	}
	else{
		Float *tmp = SCAST_FLOAT(imag_);
		assert(tmp->number_ == 0 && "Integer Expected!");
		return real_->returnInt();
	}
}

void Complex::print(){
	real_->print();
	if (imag_->type_ == RATIONAL){
		Rational *tmp = SCAST_RATIONAL(imag_);
		string num = tmp->numerator_.number_;
		if (num[0] == '0') return;
		if (num[0] != '-') printf("+");
	}
	if (imag_->type_ == FLOAT){
		Float *tmp = SCAST_FLOAT(imag_);
		if (fabs(tmp->number_) == 0) return;
		if (tmp->number_ >= 0) printf("+");
	}
	imag_->print();
	printf("i");
}

Complex *Complex::from_string(const char *expression_in_char){
	string expression = expression_in_char;
	string real, imag;
	int len = expression.size();
	if (expression[len - 1] != 'i'&&expression[len - 1] != 'I') return NULL;
	int separate_pos = -1;
	for (int i = len - 2; i >= 0; --i){
		if (expression[i] == '+' || expression[i] == '-'){
			if (i == 0){ separate_pos = 0; }
			else if (expression[i - 1] == 'e') continue;
			else { separate_pos = i; break; }
		}
	}
	if (separate_pos == -1) return NULL;
	if (separate_pos == 0){
		imag = expression.substr(0, len - 1);
		if (imag == "+" || imag == "-") imag += "1";
		return new Complex("0", imag);
	}
	else{
		imag = expression.substr(separate_pos, len - separate_pos - 1);
		if (imag == "+" || imag == "-") imag += "1";
		real = expression.substr(0, separate_pos);
		return new Complex(real, imag);
	}
}

