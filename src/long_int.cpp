#include "long_int.h"
#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

string add_positive(string str1, string str2); //only support positive number 
string subtract_positive(string str1, string str2);
string multiply_positive(string str1, string str2);
string process_divide_positive(string tmp, string str2, int count, string *p);
string divide_positive(string str1, string str2);

string add(string a, string b); //negative number included
string subtract(string str1, string str2);
string multiply(string str1, string str2);
string divide(string str1, string str2);
string mod(string str1, string str2);


LongInt::LongInt(string number) : number_(number){}

LongInt::LongInt(const LongInt &long_int2) : number_(long_int2.number_){}

LongInt::~LongInt(){}

LongInt::operator bool(){
	if (number_ == "0") return false;
	else return true;
}

LongInt::operator double(){
	double result;
	istringstream iss(number_);
	iss >> result;
	return result;
}

bool LongInt::operator!=(const LongInt &long_int2) const{
	return number_ != long_int2.number_;
}

bool LongInt::operator<(const LongInt &long_int2) const{
	if (number_[0] != '-'&&long_int2.number_[0] != '-'){ //同正 
		if (number_.size()<long_int2.number_.size() || number_.size() == long_int2.number_.size() && number_<long_int2.number_) return true;
		else return false;
	}
	if (number_[0] == '-'&&long_int2.number_[0] == '-'){ //同负 
		if (number_.size()>long_int2.number_.size() || number_.size() == long_int2.number_.size() && number_>long_int2.number_) return true;
		else return false;
	}
	if (number_[0] != '-'&&long_int2.number_[0] == '-') return false; //一正一负 
	if (number_[0] == '-'&&long_int2.number_[0] != '-') return true; //一负一正
}

bool LongInt::operator==(const LongInt &long_int2) const{
	return number_ == long_int2.number_;
}

LongInt &LongInt::operator=(const LongInt &long_int2){
	number_ = long_int2.number_;
	return *this;
}

LongInt LongInt::operator+(const LongInt &long_int2) const{
	LongInt result;
	result.number_ = add(number_, long_int2.number_);
	return result;
}

LongInt LongInt::operator-(const LongInt &long_int2) const{
	LongInt result;
	result.number_ = subtract(number_, long_int2.number_);
	return result;
}

LongInt LongInt::operator*(const LongInt &long_int2) const{
	LongInt result;
	result.number_ = multiply(number_, long_int2.number_);
	return result;
}

LongInt LongInt::operator/(const LongInt &long_int2) const{
	assert(long_int2.number_ != "0" && "devide zero");
	LongInt result;
	result.number_ = divide(number_, long_int2.number_);
	return result;
}

LongInt LongInt::operator%(const LongInt &long_int2) const{
	assert(long_int2.number_ != "0" && "devide zero");
	LongInt result;
	result.number_ = mod(number_, long_int2.number_);
	return result;
}

LongInt max(const LongInt &long_int1, const LongInt &long_int2){
	LongInt result(MAX(long_int1.number_, long_int2.number_));
	return result;
}

LongInt min(const LongInt &long_int1, const LongInt &long_int2){
	LongInt result(MIN(long_int1.number_, long_int2.number_));
	return result;
}

void LongInt::print(){
	cout << number_;
}




string add(string str1, string str2){
	if (str1[0] != '-'&&str2[0] != '-'){ //同正
		return add_positive(str1, str2);
	}
	if (str1[0] == '-'&&str2[0] == '-'){ //同负 
		string tmp = '-' + add_positive(str1.erase(0, 1), str2.erase(0, 1));
		return tmp;
	}
	if (str1[0] != '-'&&str2[0] == '-'){ //一正一负 
		return subtract_positive(str1, str2.erase(0, 1));
	}
	if (str1[0] == '-'&&str2[0] != '-'){ //一负一正
		return subtract_positive(str2, str1.erase(0, 1));
	}
}

string subtract(string str1, string str2){
	if (str1[0] != '-'&&str2[0] != '-'){ //同正
		return subtract_positive(str1, str2);
	}
	if (str1[0] == '-'&&str2[0] == '-'){ //同负 
		return subtract_positive(str2.erase(0, 1), str1.erase(0, 1));
	}
	if (str1[0] != '-'&&str2[0] == '-'){ //一正一负 
		return add_positive(str1, str2.erase(0, 1));
	}
	if (str1[0] == '-'&&str2[0] != '-'){ //一负一正
		string tmp = '-' + add_positive(str1.erase(0, 1), str2);
		return tmp;
	}
}

string add_positive(string str1, string str2){
	int len1 = str1.size(), len2 = str2.size();
	if (len1<len2){ swap(str1, str2); swap(len1, len2); } //长的作str1 
	str1 = "0" + str1;
	for (int i = 0; i<len1 - len2 + 1; ++i) str2 = "0" + str2;

	int *p = new int[len1 + 1];
	for (int i = 0; i<len1 + 1; ++i) p[i] = 0;

	for (int i = 0; i<len1; ++i){
		int tmp = (str1[len1 - i] - '0') + (str2[len1 - i] - '0') + p[len1 - i];
		if (tmp >= 10){
			p[len1 - i] = tmp % 10;
			p[len1 - i - 1] = 1;
		}
		else p[len1 - i] = tmp;
	}
	string result = "";
	if (p[0] != 0) result += p[0] + '0';
	for (int i = 1; i<len1 + 1; ++i)  result += p[i] + '0';
	delete p;
	return result;
}

string subtract_positive(string str1, string str2){
	int len1 = str1.size(), len2 = str2.size();
	bool flag; //判断谁大
	if (len1>len2 || len1 == len2&&str1 >= str2) flag = true;
	else flag = false;
	if (!flag){ swap(str1, str2); swap(len1, len2); } //确保str1>str2 
	for (int i = 0; i<len1 - len2; ++i) str2 = "0" + str2;

	int *p = new int[len1];
	for (int i = 0; i<len1; ++i) p[i] = 0;

	for (int i = len1 - 1; i >= 0; --i){
		int tmp = (str1[i] - '0') - (str2[i] - '0') + p[i];
		if (tmp<0){
			p[i] = tmp + 10;
			p[i - 1] = -1;
		}
		else p[i] = tmp;
	}
	string result = "";
	int start = len1 - 1;
	for (int i = 0; i<len1 - 1; ++i){
		if (p[i] != 0){
			start = i; break;
		}
	}
	for (int i = start; i<len1; ++i) result += p[i] + '0';
	if (!flag) result = '-' + result;
	delete p;
	return result;
}

string multiply(string str1, string str2){
	if (str1 == "0" || str2 == "0") return "0"; //防止出现0*负数为-0的情况
	if (str1[0] != '-'&&str2[0] != '-'){ //同正
		return multiply_positive(str1, str2);
	}
	if (str1[0] == '-'&&str2[0] == '-'){ //同负 
		return multiply_positive(str1.erase(0, 1), str2.erase(0, 1));
	}
	if (str1[0] != '-'&&str2[0] == '-'){ //一正一负 
		return ('-' + multiply_positive(str1, str2.erase(0, 1)));
	}
	if (str1[0] == '-'&&str2[0] != '-'){ //一负一正
		return ('-' + multiply_positive(str1.erase(0, 1), str2));
	}
}

string multiply_positive(string str1, string str2){
	int len1 = str1.size(), len2 = str2.size();
	int max = len1 + len2;

	int *p = new int[max];
	for (int i = 0; i<max; ++i) p[i] = 0;

	for (int i = 0; i<len2; ++i){
		for (int j = 0; j<len1; ++j){
			p[max - j - i - 1] += (str1[len1 - j - 1] - '0') * (str2[len2 - i - 1] - '0');
		}
	}

	for (int i = 0; i<max - 1; ++i){
		p[max - i - 2] += p[max - i - 1] / 10;
		p[max - i - 1] %= 10;
	}

	string result = "";
	if (p[0] != 0) result += p[0] + '0';
	for (int i = 1; i<max; ++i)  result += p[i] + '0';
	delete p;
	return result;
}

string divide(string str1, string str2){
	string tmp;
	if (str1[0] != '-'&&str2[0] != '-'){ //同正
		tmp = divide_positive(str1, str2);
	}
	if (str1[0] == '-'&&str2[0] == '-'){ //同负 
		tmp = divide_positive(str1.erase(0, 1), str2.erase(0, 1));
	}
	if (str1[0] != '-'&&str2[0] == '-'){ //一正一负 
		tmp = ('-' + divide_positive(str1, str2.erase(0, 1)));
	}
	if (str1[0] == '-'&&str2[0] != '-'){ //一负一正
		tmp = ('-' + divide_positive(str1.erase(0, 1), str2));
	}
	if (tmp == "-0") return "0"; //防止-0出现 
	else return tmp;
}

string divide_positive(string str1, string str2){
	int countstr1_0 = 0; //去掉前面多余的0 
	for (int i = 0; i<str1.size() - 1; ++i){
		if (str1[i] == '0') ++countstr1_0;
		else break;
	}
	str1 = str1.substr(countstr1_0, str1.size() - countstr1_0);

	int countstr2_0 = 0;
	for (int i = 0; i<str2.size() - 1; ++i){
		if (str2[i] == '0') ++countstr2_0;
		else break;
	}
	str2 = str2.substr(countstr2_0, str2.size() - countstr2_0);

	int len1 = str1.size();
	string result = "", tmp = "";
	int mark = 0, count = 0;
	while (str1.size() == str2.size() && str1 >= str2 || str1.size()>str2.size()){
		for (int i = tmp.size(); i<tmp.size() + str2.size() + 1; ++i){
			tmp += str1[i];
			count += 1;
			if (tmp.size() == str2.size() && tmp >= str2 || tmp.size()>str2.size()){
				tmp = process_divide_positive(tmp, str2, count, &result);
				str1 = tmp + str1.substr(count, str1.size() - count);
				count = tmp.size();
				break;
			}
			else result += "0";
		}
	}
	int gap = len1 - result.size();
	for (int i = 0; i<gap; ++i) result += "0";

	int count2 = 0;
	for (int i = 0; i<result.size() - 1; ++i){
		if (result[i] == '0') ++count2;
		else break;
	}
	return result.substr(count2, result.size() - count2);
}

string process_divide_positive(string tmp, string str2, int count, string *p){
	int n = str2.size() + 1;
	int *a = new int[n];
	int x;
	string b;
	bool flag = false;
	if (tmp.size()>str2.size()) flag = true;
	tmp = "0" + tmp;
	for (int i = 1; b <= tmp; ++i){ //
		b = "";
		if (flag) b += "0";
		for (int j = 0; j<n; ++j) a[j] = 0;
		for (int j = 0; j<n - 1; ++j){
			a[n - 1 - j] = (str2[n - 2 - j] - '0') * i;
		}
		for (int k = 0; k<n - 1; ++k){
			a[n - k - 2] += a[n - k - 1] / 10;
			a[n - k - 1] %= 10;
		}
		for (int j = 0; j<n; ++j){
			b += (a[j] + '0');
		}
		x = i - 1;
	}

	b = "";
	if (flag) b += "0";
	for (int j = 0; j<n; ++j) a[j] = 0;
	for (int j = 0; j<n - 1; ++j){
		a[n - 1 - j] = (str2[n - 2 - j] - '0') * x;
	}
	for (int k = 0; k<n - 1; ++k){
		a[n - k - 2] += a[n - k - 1] / 10;
		a[n - k - 1] %= 10;
	}
	for (int j = 0; j<n; ++j){
		b += (a[j] + '0');
	}
	*p += (x + '0');
	delete a;

	int *m = new int[count + 1];
	for (int i = 0; i<count + 1; ++i){
		m[i] = (tmp[i] - '0') - (b[i] - '0');
	}
	for (int i = 0; i<count; ++i){
		if (m[count - i]<0){
			m[count - i] += 10;
			m[count - i - 1] -= 1;
		}
	}
	int num = 0;
	string c = "";
	for (int i = 0; i<count + 1; ++i){
		if (m[i] == 0) ++num;
		else break;
	}
	for (int i = num; i<count + 1; ++i){c += (m[i] + '0');}
	delete m;
	return c;
}

string mod(string str1, string str2){
	string tmp = divide(str1, str2);
	return subtract(str1, multiply(str2, tmp));
}
