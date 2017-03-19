#include "opt.h"
#include "number.h"
#include "float.h"
#include "rational.h"
#include "complex.h"
#include "boolean.h"
#include "character.h"
#include "string.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <complex>
#include <cstdlib>
#define SCAST_NUMBER(x) static_cast<Number*>(x)
#define SCAST_RATIONAL(x) static_cast<Rational*>(x)
#define SCAST_FLOAT(x) static_cast<Float*>(x)
#define SCAST_BOOLEAN(x) static_cast<Boolean*>(x)
#define SCAST_CHARACTER(x) static_cast<Character*>(x)

class Add : public Opt {
    /* Use the lowest level type */
    Number *calc(Cons *con) {
		
		Number *res = new Rational("0","1");
		Number *last;	
        for (; con; con = con->cdr)
        {
			if(con->car->type_>3||con->car->type_<1)
			{
				throw 0;
			}
            Number *opr = SCAST_NUMBER(con->car), *conv;
            last = res;
            if (res->type_ > opr->type_)
			{
				res = res->add(conv = res->convert(opr));
			}
            else
			{
                res = (conv = opr->convert(res))->add(opr);
			}
			
            delete last;
            delete conv;
			
        }
        return res;
    }
};

class Sub:public Opt{
	Number *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if(tmp->car->type_>3||tmp->car->type_<1)
			{
				throw 0;
			}
			cnt++;
		}
		Number *res=new Rational("0", "1"),*last;
		Number *opr = SCAST_NUMBER(con->car), *conv;
		last=res;
		if(cnt==1)
		{
			if (res->type_ > opr->type_)
				res = res->sub(conv = res->convert(opr));
			else
				res = (conv = opr->convert(res))->sub(opr);
			delete last;
			delete conv;
			return res;
		}
		if (res->type_ > opr->type_)
            res = res->add(conv = res->convert(opr));
        else
            res = (conv = opr->convert(res))->add(opr);
        con=con->cdr;
        delete last;
        delete conv;
		for(;con;con=con->cdr)
		{
			opr=SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
				res=res->sub(conv=res->convert(opr));
			else
				res=(conv=opr->convert(res))->sub(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Mul : public Opt {
    /* Use the lowest level type */
    Number *calc(Cons *con) {
		Number *res = new Rational("1", "1"), *last;
        for (; con; con = con->cdr)
        {
			if(con->car->type_>3||con->car->type_<1)
			{
				throw 0;
			}
            Number *opr = SCAST_NUMBER(con->car), *conv;
            last = res;
            if (res->type_ > opr->type_)
                res = res->mul(conv = res->convert(opr));
            else
                res = (conv = opr->convert(res))->mul(opr);
            delete last;
            delete conv;
        }
        return res;
    }
};

class Div:public Opt{
	Number *calc(Cons *con)
	{
		Cons *tmp=con;
		int cnt=0;
		for(;tmp;tmp=tmp->cdr)
		{
			if(tmp->car->type_>3||tmp->car->type_<1)
			{
				throw 0;
			}
			cnt++;
		}
		Number *res=new Rational("1","1"),*last;
		Number *opr=SCAST_NUMBER(con->car),*conv;
		last=res;
		Number *zero = new Float(0.0);
		if(cnt==1)
		{
			if(res->type_>opr->type_)
				res=res->div(conv=res->convert(opr));
			else
				res=(conv=opr->convert(res))->div(opr);
			delete last;
			delete conv;
			return res;
		}
		if(res->type_>opr->type_)
			res=res->mul(conv=res->convert(opr));
		else
			res=(conv=opr->convert(res))->mul(opr);
		con=con->cdr;
		delete last;
		delete conv;
		for(;con;con=con->cdr)
		{
			opr = SCAST_NUMBER(con->car);
			last=res;
			if(res->type_>opr->type_)
				res=res->div(conv=res->convert(opr));
			else
				res=(conv=opr->convert(res))->div(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Abs:public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->abs();
	}
};

class Quotient:public Opt{
	Number *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		Number *res = SCAST_NUMBER(con->car), *opr = SCAST_NUMBER(con->cdr->car), *last, *conv;
		if (res->type_<1 || res->type_>3) throw 0;
		if (opr->type_<1 || opr->type_>3) throw 0;
		last = res;
		if (res->type_ > opr->type_)
			res = res->quo(conv = res->convert(opr));
		else
			res = (conv = opr->convert(res))->quo(opr);
		delete last;
		delete conv;
		return res;
	}
};

class Remainder:public Opt{
	Number *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		Number *res = SCAST_NUMBER(con->car), *opr = SCAST_NUMBER(con->cdr->car), *last, *conv;
		if (res->type_<1 || res->type_>3) throw 0;
		if (opr->type_<1 || opr->type_>3) throw 0;
		last = res;
		if (res->type_ > opr->type_)
			res = res->rem(conv = res->convert(opr));
		else
			res = (conv = opr->convert(res))->rem(opr);
		delete last;
		delete conv;
		return res;
	}
};

class Modulo :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		Number *res = SCAST_NUMBER(con->car), *opr = SCAST_NUMBER(con->cdr->car), *last, *conv;
		if (res->type_<1 || res->type_>3) throw 0;
		if (opr->type_<1 || opr->type_>3) throw 0;
		last = res;
		if (res->type_ > opr->type_)
			res = res->mod(conv = res->convert(opr));
		else
			res = (conv = opr->convert(res))->mod(opr);
		delete last;
		delete conv;
		return res;
	}
};

class Gcd :public Opt{
	Number *calc(Cons *con) {
		Number *res = new Rational("0", "1");
		Number *last;
		for (; con; con = con->cdr){
			if (con->car->type_>3 || con->car->type_<1) throw 0;
			Number *opr = SCAST_NUMBER(con->car), *conv;
			last = res;
			if (res->type_ > opr->type_) res = res->gcd(conv = res->convert(opr));
			else res = (conv = opr->convert(res))->gcd(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Lcm :public Opt{
	Number *calc(Cons *con) {
		Number *res = new Rational("1", "1");
		Number *last;
		for (; con; con = con->cdr){
			if (con->car->type_>3 || con->car->type_<1) throw 0;
			Number *opr = SCAST_NUMBER(con->car), *conv;
			last = res;
			if (res->type_ > opr->type_) res = res->lcm(conv = res->convert(opr));
			else res = (conv = opr->convert(res))->lcm(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Expt :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		Number *res = SCAST_NUMBER(con->car), *opr = SCAST_NUMBER(con->cdr->car), *last, *conv;
		if (res->type_<1 || res->type_>3) throw 0;
		if (opr->type_<1 || opr->type_>3) throw 0;
		last = res;
		if (res->type_ > opr->type_)
			res = res->expt(conv = res->convert(opr));
		else
			res = (conv = opr->convert(res))->expt(opr);
		delete last;
		delete conv;
		return res;
	}
};

class Sqrt :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->sqt();
	}
};

class Numerator :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->num();
	}
};

class Denominator :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->den();
	}
};

class Floor :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->flo();
	}
};

class Ceiling :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->cei();
	}
};

class Truncate :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->tru();
	}
};

class Round :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->rou();
	}
};

class Toexact :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->toexact();
	}
};

class Toinexact :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->toinexact();
	}
};

class Max :public Opt {
	Number *calc(Cons *con){
		Number *res = SCAST_NUMBER(con->car);
		Number *last;
		for (; con; con = con->cdr){
			if (con->car->type_>3 || con->car->type_<1) throw 0;	
			Number *opr = SCAST_NUMBER(con->car), *conv;
			last = res;
			if (res->type_ > opr->type_) res = res->maxi(conv = res->convert(opr));
			else res = (conv = opr->convert(res))->maxi(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Min :public Opt {
	Number *calc(Cons *con){
		Number *res = SCAST_NUMBER(con->car);
		Number *last;
		for (; con; con = con->cdr){
			if (con->car->type_>3 || con->car->type_<1) throw 0;
			Number *opr = SCAST_NUMBER(con->car), *conv;
			last = res;
			if (res->type_ > opr->type_) res = res->mini(conv = res->convert(opr));
			else res = (conv = opr->convert(res))->mini(opr);
			delete last;
			delete conv;
		}
		return res;
	}
};

class Sin :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->sinx();
	}
};

class Cos :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->cosx();
	}
};

class Tan :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->tanx();
	}
};

class Asin :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->asinx();
	}
};

class Acos :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->acosx();
	}
};

class Atan :public Opt{
	Number *calc(Cons *con){
		if (con->cdr == NULL){
			if (con->car->type_<1 || con->car->type_>3) throw 0;
			return SCAST_NUMBER(con->car)->atanx();
		}
		else{
			assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
			if (con->car->type_ < 1 || con->car->type_>3 || con->cdr->car->type_<1 || con->cdr->car->type_>3) throw 0;
			Number *first = SCAST_NUMBER(con->car), *second = SCAST_NUMBER(con->cdr->car), *conv, *res;
			if (first->type_>second->type_) res = first->atanxy(conv = first->convert(second));
			else res = (conv = second->convert(first))->atanxy(second);
			delete conv;
			return res;
		}
	}
};

class Exp :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->expx();
	}
};

class Log :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->logx();
	}
};

class Make_rectangular :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		Number *res = SCAST_NUMBER(con->car), *opr = SCAST_NUMBER(con->cdr->car), *last, *conv;
		if (res->type_<1 || res->type_>2) throw 0;
		if (opr->type_<1 || opr->type_>2) throw 0;
		last = res;
		if (res->type_ > opr->type_)
			res = res->makerectangular(conv = res->convert(opr));
		else
			res = (conv = opr->convert(res))->makerectangular(opr);
		delete last;
		delete conv;
		return res;
	}
};

class Make_polar :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		Number *res = SCAST_NUMBER(con->car), *opr = SCAST_NUMBER(con->cdr->car), *last, *conv;
		if (res->type_<1 || res->type_>2) throw 0;
		if (opr->type_<1 || opr->type_>2) throw 0;
		last = res;
		if (res->type_ > opr->type_)
			res = res->makepolar(conv = res->convert(opr));
		else
			res = (conv = opr->convert(res))->makepolar(opr);
		delete last;
		delete conv;
		return res;
	}
};

class Realpart :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->realpart();
	}
};

class Imagpart :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->cdr != NULL) throw 0;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->imagpart();
	}
};

class Magnitude :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->mag();
	}
};

class Angle :public Opt{
	Number *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->ang();
	}
};

class Less :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Number *first, *second, *conv;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_<1 || con->cdr->car->type_>3) throw 0;
			first = SCAST_NUMBER(con->car), second = SCAST_NUMBER(con->cdr->car);
			last = res;
			if (first->type_>second->type_) res = first->less(conv = first->convert(second));
			else res = (conv = second->convert(first))->less(second);
			delete last;
			delete conv;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class Lessequal :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res=new Boolean(), *last;
		Number *first, *second, *conv;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_<1 || con->cdr->car->type_>3) throw 0;
			first = SCAST_NUMBER(con->car), second = SCAST_NUMBER(con->cdr->car);
			last = res;
			if (first->type_>second->type_) res = first->lessequal(conv = first->convert(second));
			else res = (conv = second->convert(first))->lessequal(second);
			delete last;
			delete conv;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class Greater :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Number *first, *second, *conv;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_<1 || con->cdr->car->type_>3) throw 0;
			first = SCAST_NUMBER(con->car), second = SCAST_NUMBER(con->cdr->car);
			last = res;
			if (first->type_>second->type_) res = first->greater(conv = first->convert(second));
			else res = (conv = second->convert(first))->greater(second);
			delete last;
			delete conv;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class Greaterequal :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Number *first, *second, *conv;
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_<1 || con->cdr->car->type_>3) throw 0;
			first = SCAST_NUMBER(con->car), second = SCAST_NUMBER(con->cdr->car);
			last = res;
			if (first->type_>second->type_) res = first->greaterequal(conv = first->convert(second));
			else res = (conv = second->convert(first))->greaterequal(second);
			delete last;
			delete conv;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class IsZero :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isZero();
	}
};

class IsNegative :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isNegative();
	}
};

class IsPositive :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isPositive();
	}
};

class IsOdd :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isOdd();
	}
};

class IsEven :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isEven();
	}
};

class IsInteger :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isInteger();
	}
};

class IsRational :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isRational();
	}
};

class IsReal :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isReal();
	}
};

class IsComplex :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isComplex();
	}
};

class IsNumber :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isNumber();
	}
};

class IsChar :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isChar();
	}
};

class IsString :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>6) throw 0;
		return (con->car)->isString();
	}
};

class IsExact :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isExact();
	}
};

class IsInexact :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_<1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->isInexact();
	}
};

class CharIsEqual :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Character *first, *second;
		if (con->car->type_ != 5) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 5) throw 0;
			first = SCAST_CHARACTER(con->car), second = SCAST_CHARACTER(con->cdr->car);
			last = res;
			res = first->charIsEqual(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class CharIsCiEqual :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Character *first, *second;
		if (con->car->type_ != 5) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 5) throw 0;
			first = SCAST_CHARACTER(con->car), second = SCAST_CHARACTER(con->cdr->car);
			last = res;
			res = first->charIsCiEqual(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class CharLess :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Character *first, *second;
		if (con->car->type_ != 5) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 5) throw 0;
			first = SCAST_CHARACTER(con->car), second = SCAST_CHARACTER(con->cdr->car);
			last = res;
			res = first->charLess(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class CharCiLess :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Character *first, *second;
		if (con->car->type_ != 5) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 5) throw 0;
			first = SCAST_CHARACTER(con->car), second = SCAST_CHARACTER(con->cdr->car);
			last = res;
			res = first->charCiLess(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class CharLessequal :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Character *first, *second;
		if (con->car->type_ != 5) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 5) throw 0;
			first = SCAST_CHARACTER(con->car), second = SCAST_CHARACTER(con->cdr->car);
			last = res;
			res = first->charLessequal(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class CharCiLessequal :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		Character *first, *second;
		if (con->car->type_ != 5) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 5) throw 0;
			first = SCAST_CHARACTER(con->car), second = SCAST_CHARACTER(con->cdr->car);
			last = res;
			res = first->charCiLessequal(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class CharIsAlpha :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charIsAlpha();
	}
};

class CharIsNum :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charIsNum();
	}
};

class CharIsSpace :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charIsSpace();
	}
};

class CharIsUpCase :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charIsUpCase();
	}
};

class CharIsLowCase :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charIsLowCase();
	}
};

class CharToInt :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charToInt();
	}
};

class IntToChar :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ <1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->intToChar();
	}
};

class CharToUpCase :public Opt{
	Character *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charToUpCase();
	}
};

class CharToDownCase :public Opt{
	Character *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 5) throw 0;
		return SCAST_CHARACTER(con->car)->charToDownCase();
	}
};

class StrIsEqual :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		String *first, *second;
		if (con->car->type_ != 6) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 6) throw 0;
			first = SCAST_STRING(con->car), second = SCAST_STRING(con->cdr->car);
			last = res;
			res = first->strIsEqual(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class StrIsCiEqual :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		String *first, *second;
		if (con->car->type_ != 6) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 6) throw 0;
			first = SCAST_STRING(con->car), second = SCAST_STRING(con->cdr->car);
			last = res;
			res = first->strIsCiEqual(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class StrLess :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		String *first, *second;
		if (con->car->type_ != 6) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 6) throw 0;
			first = SCAST_STRING(con->car), second = SCAST_STRING(con->cdr->car);
			last = res;
			res = first->strLess(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class StrCiLess :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		String *first, *second;
		if (con->car->type_ != 6) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 6) throw 0;
			first = SCAST_STRING(con->car), second = SCAST_STRING(con->cdr->car);
			last = res;
			res = first->strCiLess(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class StrLessequal :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		String *first, *second;
		if (con->car->type_ != 6) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 6) throw 0;
			first = SCAST_STRING(con->car), second = SCAST_STRING(con->cdr->car);
			last = res;
			res = first->strLessequal(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class StrCiLessequal :public Opt{
	Boolean *calc(Cons *con){
		Boolean *res = new Boolean(), *last;
		String *first, *second;
		if (con->car->type_ != 6) throw 0;
		for (; con->cdr; con = con->cdr){
			if (con->cdr->car->type_ != 6) throw 0;
			first = SCAST_STRING(con->car), second = SCAST_STRING(con->cdr->car);
			last = res;
			res = first->strCiLessequal(second);
			delete last;
			if (res->value_ == false) break;
		}
		return res;
	}
};

class StrLen :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 6) throw 0;
		return SCAST_STRING(con->car)->strLen();
	}
};

class StrRef :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		if (con->car->type_ != 6 || con->cdr->car->type_ <1 || con->cdr->car->type_ >3) throw 0;
		String *first = SCAST_STRING(con->car); 
		Number *second = SCAST_NUMBER(con->cdr->car);
		return first->strRef(second);
	}
};



class StrToNum :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 6) throw 0;
		return SCAST_STRING(con->car)->strToNum();
	}
};

class NumToStr :public Opt{
	Datatype *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ <1 || con->car->type_>3) throw 0;
		return SCAST_NUMBER(con->car)->numToStr();
	}
};

class SubStr :public Opt{
	String *calc(Cons *con){
		assert(con->cdr->cdr->cdr == NULL && "expected number of arguments is 3");
		if (con->car->type_ != 6 || con->cdr->car->type_ <1 || con->cdr->car->type_>3 || con->cdr->cdr->car->type_ <1 || con->cdr->cdr->car->type_>3) throw 0;
		Number *start = SCAST_NUMBER(con->cdr->car), *end = SCAST_NUMBER(con->cdr->cdr->car);
		return SCAST_STRING(con->car)->subStr(start, end);
	}
};

class StrAppend :public Opt {
	String *calc(Cons *con){
		String *res = new String(""), *last;
		for (; con; con = con->cdr){
			if (con->car->type_ != 6) throw 0;
			String *opr = SCAST_STRING(con->car);
			last = res;
			res = res->strAppend(opr);
			delete last;
		}
		return res;
	}
};

class StrCopy :public Opt{
	String *calc(Cons *con){
		assert(con->cdr == NULL && "expected number of arguments is 1");
		if (con->car->type_ != 6) throw 0;
		return SCAST_STRING(con->car)->strCopy();
	}
};

class MakeStr :public Opt{
	Datatype *calc(Cons *con){
		if (con->car->type_ <1 || con->car->type_>3) throw 0;
		if (con->cdr == NULL){
			Number *first = SCAST_NUMBER(con->car);
			Character *second = new Character('\0');
			return String::makeStr(first, second);
		}
		else if (con->cdr->cdr == NULL){
			if (con->cdr->car->type_ != 5) throw 0;
			Number *first = SCAST_NUMBER(con->car);
			Character *second = SCAST_CHARACTER(con->cdr->car);
			return String::makeStr(first, second);
		}
		else throw 0;
	}
};

class GenerateStr :public Opt {
	String *calc(Cons *con){
		String *res = new String(""), *last;
		for (; con; con = con->cdr){
			if (con->car->type_ != 5) throw 0;
			Character *opr = SCAST_CHARACTER(con->car);
			last = res;
			res = res->generateStr(opr);
			delete last;
		}
		return res;
	}
};

class IsEqual :public Opt{
	Boolean *calc(Cons *con){
		assert(con->cdr->cdr == NULL && "expected number of arguments is 2");
		if (con->car->type_<1 || con->car->type_>6 || con->cdr->car->type_<1 || con->cdr->car->type_>6) throw 0;
		if (con->car->type_ >= 1 && con->car->type_ <= 3 && con->cdr->car->type_ >= 1 && con->cdr->car->type_ <= 3){
			Boolean *res;
			Number *first, *second, *conv;
			first = SCAST_NUMBER(con->car), second = SCAST_NUMBER(con->cdr->car);
			if (first->type_>second->type_) res = first->isEqual(conv = first->convert(second));
			else res = (conv = second->convert(first))->isEqual(second);
			delete conv;
			return res;
		}
		else if (con->car->type_ != con->cdr->car->type_) return new Boolean(false);
		else if (con->car->type_ == 4){
			Boolean *first = SCAST_BOOLEAN(con->car), *second = SCAST_BOOLEAN(con->cdr->car);
			return first->isEqual(second);
		}
		else if (con->car->type_ == 5){
			Character *first = SCAST_CHARACTER(con->car), *second = SCAST_CHARACTER(con->cdr->car);
			return first->charIsEqual(second);
		}
		else{
			String *first = SCAST_STRING(con->car), *second = SCAST_STRING(con->cdr->car);
			return first->strIsEqual(second);
		}
	}
};