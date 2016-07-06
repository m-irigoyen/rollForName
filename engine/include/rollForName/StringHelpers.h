#pragma once

#include <functional>
#include <cctype>
#include <locale>
#include <algorithm>

#include "typedefs.h"

namespace rfn
{

	// GENERIC STUFF

//! trim from start (in place)
template <typename T>
static inline void triml(T &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

//! trim from end (in place)
template <typename T>
static inline void trimr(T &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

//! trim from both ends (in place)
template <typename T>
static inline void trim(T &s) {
	triml(s);
	trimr(s);
}

//! trim from start (copying)
template <typename T>
static inline ustring trimmedl(T s) {
	triml(s);
	return s;
}

//! trim from end (copying)
template <typename T>
static inline ustring trimmedr(T s) {
	trimr(s);
	return s;
}

//! trim from both ends (copying)
template <typename T>
static inline ustring trimmed(T s) {
	trim(s);
	return s;
}

template <typename T>
static inline void removeSpaces(T& s) {
	s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
}

//! Returns true if s contains searched
static inline bool contains(ustring s, ustring searched)
{
	return s.find(searched) != ustring::npos;
}

static inline bool compare(ustring s1, ustring s2)
{
	return s1.compare(s2) == 0;
}

static inline void makeValidIdInPlace(ustring& s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

static inline ustring makeValidId(const ustring& s)
{
	ustring copy = s;
	std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	return copy;
}

static inline ustring stringToUstring(const std::string& s)
{
	ustring result;
	result.assign(s.begin(), s.end());
	return result;
}

static inline ustring tabFromLevel(const int level)
{
	ustring tab;
	for (int i = 0; i < level; ++i)
	{
		tab += L"\t";
	}
	return tab;
}



//
//// to_string
//template <typename T>
//ustring to_ustring(T value)
//{
//	std::string s = std::to_string(value);
//	ustring result;
//	fromUTF8(s, result);
//	return result;
//}

//
//ustring to_ustring(long long value)
//{
//	std::string s = std::to_string(value);
//	ustring result;
//	fromUTF8(s, result);
//	return result;
//}
//
//ustring to_ustring(unsigned value)
//{
//	std::string s = std::to_string(value);
//	ustring result;
//	fromUTF8(s, result);
//	return result;
//}
//
//ustring to_ustring(unsigned long value);
//ustring to_ustring(unsigned long long value);
//ustring to_ustring(float value);
//ustring to_ustring(double value);
//ustring to_ustring(long double value);

}
