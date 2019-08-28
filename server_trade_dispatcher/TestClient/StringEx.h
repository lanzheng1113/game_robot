#pragma once
#include <string>
class String : public std::string {
public:
	/**
	 * Null string.
	 *
	 * Rather than to code something like this:
	 * if (myString == "")
	 * it's better to write:
	 * if (myString.empty())
	 * return "" -> return String::null
	 */
	 static const char * null;

	 String() : std::string() { }

	 String(const char * str) : std::string(str) { }

	 String(const std::string & str) : std::string(str) { }

	/**
	 * Converts this String to std::string.
	 *
	 * @return the converted String to std::string
	 */
	 operator const std::string&() {
		return *this;
	}

	/**
	 * Converts this string to an int.
	 *
	 * @return the string converted to an int or 0 if failed to convert
	 */
	 int toInteger() const;
	/**
	 * Converts this string to a boolean.
	 *
	 * Detects strings: "true", "TRUE", "yes", "YES", "1"
	 *                  "false", "FALSE", "no", "NO", "0"
	 * Be very carefull when using this method, maybe it should throw an Exception.
	 *
	 * @return the string converted to a boolean (return false if failed to convert)
	 */
	 bool toBoolean() const;
	
	 /**
	 * Gets a string from a number.
	 *
	 * @param number number to convert into a string
 	 * @param minLength minimal length of the string (i.e. fromNumber(15, 3) => "015")
	 * @return number converted to a string
	 */
	 static std::string fromNumber(int number, int minLength = 0);

	/**
	 * Gets a string from a boolean.
	 *
	 * @param boolean boolean to convert into a string
	 * @return boolean converted to a string
	 */
	 static std::string fromBoolean(bool boolean);

	/**
	 * Gets a string from an unsigned int.
	 *
	 * @param number unsigned int to convert into a string
	 * @return unsigned int converted to a string
	 */
	static std::string fromUnsignedInt(unsigned int number);

	/**
	 * Gets a string from an double.
	 *
	 * @param number double to convert into a string
	 * @return double converted to a string
	 */
	 static std::string fromDouble(double number);

	/**
	 * Gets a string from a long.
	 *
	 * @param number long to convert into a string
	 * @return long converted to a string
	 */
	 static std::string fromLong(long number);

	/**
	 * Gets a string from a long long.
	 *
	 * @param number long long to convert into a string
	 * @return long long converted to a string
	 */
	 static std::string fromLongLong(long long number);

	/**
	 * Gets a string from a unsigned long long.
	 *
	 * @param number unsigned long long to convert into a string
	 * @return unsigned long long converted to a string
	 */
	 static std::string fromUnsignedLongLong(unsigned long long number);
	/**
	 * Converts all of the characters in this string to lower case.
	 * @return the string converted to lowercase
	 */
	 std::string toLowerCase() const;

	/**
	 * Converts all of the characters in this string to upper case.
	 * @return the string converted to uppercase
	 */
	 std::string toUpperCase() const;
};