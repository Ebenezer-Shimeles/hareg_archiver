#pragma once
#include <common/hareg_error.h>
template<class T>
class HaregVal{
public:
	HaregError err;
	bool is_error;
	T value;
	
	HaregVal(T&& v) : value(std::move(v) ), is_error(false){} //move constructor
	HaregVal(HaregError e) : err(e), is_error(true){}
	HaregVal(const T& v): value(v), is_error(false){} //copy constructor
	
};

//specialized for void and has no data value
template<>
class HaregVal<void>{
public:
	HaregError err;
	bool is_error;
	HaregVal(void): is_error(false){}
	HaregVal(HaregError e) : err(e), is_error(true){}
};