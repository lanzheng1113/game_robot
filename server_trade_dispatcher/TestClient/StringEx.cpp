#include "stdafx.h"
#include "StringEx.h"
#include <cctype>
#include <sstream>
#include <algorithm>
using namespace std;

int String::toInteger() const
{
	int tmp = 0;

	stringstream ss(c_str());
	ss >> tmp;

	return tmp;
}

bool String::toBoolean() const
{
	String tmp(c_str());
	tmp = tmp.toLowerCase();
	if (tmp == "true" || tmp == "yes" || tmp == "1") {
		return true;
	}

	return false;
}

std::string String::toUpperCase() const {
	string tmp(c_str());
	transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int)) toupper);
	return tmp;
}

std::string String::toLowerCase() const {
	string tmp(c_str());
	transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int)) tolower);
	return tmp;
}

std::string String::fromNumber( int number, int minLength /*= 0*/ )
{
	if (number < 0) {
		return "-" + fromNumber((-number), minLength - 1);
	}

	minLength = (minLength < 0) ? 0 : minLength;

	stringstream ss;
	std::locale cloc("C");
	ss.imbue(cloc);

	ss << number;

	std::string result = ss.str();
	while (result.length() < (unsigned int) minLength) {
		result = "0" + result;
	}
	return result;
}

std::string String::fromBoolean( bool boolean )
{
	stringstream ss;
	ss << boolean;
	return ss.str();
}

std::string String::fromUnsignedInt( unsigned int number )
{
	stringstream ss;
	std::locale cloc("C");
	ss.imbue(cloc);
	ss << number;
	return ss.str();
}

std::string String::fromDouble( double number )
{
	stringstream ss;
	std::locale cloc("C");
	ss.imbue(cloc);
	ss << number;
	return ss.str();
}

std::string String::fromLong( long number )
{
	stringstream ss;

	std::locale cloc("C");
	ss.imbue(cloc);

	ss << number;
	return ss.str();
}

std::string String::fromLongLong( long long number )
{
	stringstream ss;
	std::locale cloc("C");
	ss.imbue(cloc);
	ss << number;
	return ss.str();
}

std::string String::fromUnsignedLongLong( unsigned long long number )
{
	stringstream ss;
	std::locale cloc("C");
	ss.imbue(cloc);
	ss << number;
	return ss.str();
}
