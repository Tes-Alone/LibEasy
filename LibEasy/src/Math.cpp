
/*
 * File name: Math.cpp
 * 
 * Description:
 *		This is a c++ implement file.
 *
 * Start Date: 2019年10月03日 18时54分21秒 周4
 * End Date:
 *
 * Version: 1.0
 *
 * Author: Tes Alone.
*/

#include <cmath> // FIXME

#include "Math.hpp"
#include "Integer.hpp"
#include "Float.hpp"
#include "AsmLib.hpp"

#include "Console.hpp"

#include "Exception.tcc"

namespace Easy {
	
e_int Math::abs(e_int num) 
{
 	if (num==Integer::MIN) 
		throw NotInFunctionDomainException{__func__, __LINE__, Integer{num}}; 
	return num<0 ? -num : num;	
}

e_long Math::abs(e_long num) 
{
	if (num==Long::MIN) 
		throw  NotInFunctionDomainException{__func__, __LINE__, Long{num}}; 
	return num<0 ? -num : num;	
}

e_int Math::min(e_int n1, e_int n2)
{
	return n1>n2 ? n2 : n1;
}

e_int Math::max(e_int n1, e_int n2)
{
	return n1<n2 ? n2 : n1;
}

e_float Math::abs(e_float f)
{
	return f<0.0f ? -f : f;
}

e_double Math::abs(e_double d) 
{
	return d<0.0 ? -d : d;
}

static e_double facts[22] = {
	1.0, 
	1.0, 
	2.0, 
	6.0,
	24.0,
	120.0,
	720.0,
	5040.0,
	40320.0, 
	362880.0, 
	3628800.0, 
	39916800.0, 
	479001600.0, 
	6227020800.0,
	87178291200.0, 
	1307674368000.0,
	20922789888000.0,
	355687428096000.0,
	6402373705728000.0,
	121645100408832000.0, //19
	2432902008176640000.0,
	51090942171709440000.0,
};

e_float Math::pow(e_float base, e_float exp) { //FIXME
	return ::powf(base, exp);
}

e_double Math::pow(e_double base, e_double exp) { //FIXME
	return ::pow(base, exp);
}

e_float Math::epow(e_float exp) {
	e_float _pow = 1;
	e_float r = 1;
	
	for (e_int i=1; i<21; i++)
	{
		_pow = _pow * exp;
		r = r + (_pow/facts[i]);
	}
	
	return r; 
}

e_double Math::epow(e_double exp) {
	e_double _pow = 1;
	e_double r = 1;
	
	for (e_int i=1; i<21; i++)
	{
		_pow = _pow * exp;
		r = r + (_pow/facts[i]);
	}
	
	return r; 
}

e_float Math::log10(e_float f) { //FIXME
	return ::log10f(f);
}

e_double Math::log10(e_double d) { //FIXME
	return ::log10(d);
}


e_float _logMax(e_float f) {
	Console::outln((1.0f/(10.0f-1.0f)) * f);
	return ((1.0f/(10.0f-1.0f))*f) - (1.0f/(10.0f-1.0f));
}

e_float Math::ln(e_float f) {
	if (f < 0.0f) {
		throw  NotInFunctionDomainException{__func__, __LINE__, Float{f}};
	}
	return _logMax(f) / _logMax(Float::e());
}

e_double Math::ln(e_double d) {
	return Math::log10(d) / Math::log10(Double::e());
}

e_float Math::log2(e_float f) {
	return Math::log10(f) / Math::log10(2.0f);
}

e_double Math::log2(e_double d) {
	return Math::log10(d) / Math::log10(2.0);
}

e_float Math::sin(e_float f) {
	return Math::cos(f-(Float::PI()/2.0f));
}

e_double Math::sin(e_double d) {
	return Math::cos(d-(Double::PI()/2.0));
}

static e_double _fmod(e_double x, e_double m)
{
	e_double n = x / m;
	return x - (asm_round0(n) * m);
}

e_float Math::cos(e_float f) {
	e_float towPI = 2*Float::PI();
	f = Math::abs(f);
	f = _fmod(f, towPI);
	
	if (Float::compare(f, Float::PI()) > 0) {
		f = f - towPI;
	}
	
	e_float _pow  = f * f;
	e_float __pow = _pow;
	e_float r = 1 - _pow / facts[2];
	
	for (e_int i=4, j=1; i<21; i+=2, j*=-1)
	{
		__pow = __pow * _pow;
		r = r + (j * (__pow/facts[i]));
	}
	
	return r; 
}

e_double Math::cos(e_double d) {
	e_double towPI = 2*Double::PI();
	d = Math::abs(d);
	d = _fmod(d, towPI);
	
	if (Double::compare(d, Double::PI()) > 0) {
		d = d - towPI;
	}

	e_double _pow  = d * d;
	e_double __pow = _pow;
	e_double r = 1 - _pow / facts[2];
	
	for (e_int i=4, j=1; i<21; i+=2, j*=-1)
	{
		__pow = __pow * _pow;
		r += (j * (__pow/facts[i]));
	}
	
	return r;
}

e_float Math::tan(e_float f) {//FIXME
	return ::tanf(f);
}

e_double Math::tan(e_double d) {//FIXME
	return ::tan(d);
}

e_float Math::cot(e_float f) {//FIXME
	return 1.0f / Math::tan(f);
}

e_double Math::cot(e_double d) {//FIXME
	return 1.0 / Math::tan(d);
}

e_float Math::asin(e_float f) {//FIXME
	return ::asinf(f);
}

e_double Math::asin(e_double d) {//FIXME
	return ::asin(d);
}

e_float Math::acos(e_float f) {//FIXME
	return ::acosf(f);
}

e_double Math::acos(e_double d) {//FIXME
	return ::acos(d);
}

e_float Math::atan(e_float f) {//FIXME
	return ::atanf(f);
}

e_double Math::atan(e_double d) {//FIXME
	return ::atan(d);
}

e_float Math::acot(e_float f) {//FIXME
	return 1.0f / Math::atan(f);
}

e_double Math::acot(e_double d) {//FIXME
	return 1.0 / Math::atan(d);
}

e_float Math::sqrt(e_float f) {//FIXME
	return ::sqrtf(f);
}

e_double Math::sqrt(e_double d) {//FIXME
	return ::sqrt(d);
}

e_float Math::floor(e_float f){//FIXME
	return ::floorf(f);
}

e_double Math::floor(e_double d){//FIXME
	return ::floor(d);
}

e_float Math::ceil(e_float f) {//FIXME
	return ::ceilf(f);
}

e_double Math::ceil(e_double d) {//FIXME
	return ::ceil(d);
}

e_float Math::mod(e_float f, e_float m) {
	return _fmod(Math::abs(f), m);
}

e_double Math::mod(e_double d, e_double m) {
	return _fmod(Math::abs(d), m);
}

e_bool Math::nextBool() {
	return (rand() % 2) == 0;
}

e_char Math::nextChar() {
	return (e_char)(rand() & 0x0000ffff);
}

e_byte Math::nextByte() {
	return (e_byte)((Math::abs(rand())) % 256);
}

e_short Math::nextShort(e_short max) {
	if (max <= 0) throw IllegalArgumentException{__func__, __LINE__};
	
	e_int tmp1 = rand(); //FIXME
	e_int tmp2 = rand();
	return (e_short) ((tmp2%2==0) ? (Math::abs(tmp1*tmp2)%max) : -(Math::abs(tmp1*tmp2)%max));
}

e_int Math::nextInt(e_int max)
{
	if (max <= 0) throw IllegalArgumentException{__func__, __LINE__};
	
	e_int tmp1 = rand(); //FIXME
	e_int tmp2 = rand();
	return (tmp2%2==0) ? (Math::abs(tmp1*tmp2)%max) : -(Math::abs(tmp1*tmp2)%max);
}

e_long Math::nextLong(e_long max) {
	if (max <= 0) throw IllegalArgumentException{__func__, __LINE__};
	
	e_long tmp1 = rand(); //FIXME
	e_long tmp2 = rand();
	e_long tmp3 = rand();
	e_long tmp4 = rand();
	return (tmp2%2==0) ? (Math::abs(tmp1*tmp2*tmp3+tmp4)%max) : -(Math::abs(tmp1*tmp2*tmp3+tmp4)%max);
}

e_float Math::nextFloat() {
	return (0.5f / 1000000) * Math::nextInt(1000000) + 0.5f;
}

e_double Math::nextDouble() {
	return (0.5 / 100000000) * Math::nextLong(100000000) + 0.5;
}

}