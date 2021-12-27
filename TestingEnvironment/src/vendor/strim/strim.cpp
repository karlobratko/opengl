#include "strim.h"

#include <iterator>

void ltrim(std::string& str) {
	str.erase(
		std::begin(str),
		std::find_if(
			std::begin(str),
			std::end(str),
			[](const unsigned char c) {
				return !std::isspace(c);
			}
		)
	);
}

void rtrim(std::string& str) {
	str.erase(
		std::find_if(
			std::rbegin(str),
			std::rend(str),
			[](const unsigned char c) {
				return !std::isspace(c);
			}
		).base(),
				std::end(str)
				);
}

void trim(std::string& str) {
	ltrim(str);
	rtrim(str);
}