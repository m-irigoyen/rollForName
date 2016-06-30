#pragma once

#include <functional>
#include <cctype>
#include <locale>
#include <algorithm>

#include "typedefs.h"

namespace rfn
{

	// Encoding conversion
//template <typename T>
//std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source)
//{
//	std::string result;
//
//	std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
//	result = convertor.to_bytes(source);
//
//	return result;
//}
//
//template <typename T>
//void fromUTF8(const std::string& source, std::basic_string<T, std::char_traits<T>, std::allocator<T>>& result)
//{
//	std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
//	result = convertor.from_bytes(source);
//}

//ustring to_ustring(std::string s)
//{
//	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
//	return utf16conv.from_bytes(s);
//}



//std::string wstring_to_string(ustring string)
//{
//	// wide to UTF-8
//	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
//	return conv1.to_bytes(string);
//}

//static inline bool isSpace(const char32_t c)
//{
//	if ((c == ' ')
//		|| (c == '\t')
//		|| (c == '\v')
//		|| (c == '\n')
//		|| (c == '\f')
//		|| (c == '\r')
//		)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

//	//UTF8_STRING
////! trim from beginning (in place)
//static inline void triml(ustring &s)
//{
//	if (s.empty())
//	{
//		return;
//	}
//
//	int nb = 0;
//	ustring::iterator it = s.begin();
//	while ((it != s.end())
//		&& isSpace(*it))
//	{
//		++nb;
//		++it;
//	}
//
//	if (nb > 0)
//	{
//		s.erase(0, nb);
//	}
//}
//
////! trim from end(in place)
//static inline void trimr(ustring &s)
//{
//	if (s.empty())
//	{
//		return;
//	}
//
//	int i = s.size()-1;
//	while ((i >= 0)
//		&& std::isspace(s[i]))
//	{
//		s.pop_back();
//	}
//}
//
////! trim from both ends (in place)
//static inline void trim(ustring &s) {
//	triml(s);
//	trimr(s);
//}
//
////! trim from start (copying)
//static inline ustring trimmedl(ustring s) {
//	triml(s);
//	return s;
//}
//
////! trim from end (copying)
//static inline ustring trimmedr(ustring s) {
//	trimr(s);
//	return s;
//}
//
////! trim from both ends (copying)
//static inline ustring trimmed(ustring s) {
//	trim(s);
//	return s;
//}

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
