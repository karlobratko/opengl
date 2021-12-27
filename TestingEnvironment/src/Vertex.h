#pragma once

#include <sstream>
#include <string>

#include "strim/strim.h"

using ubyte = unsigned char;
using ushort = unsigned short;

struct Vertex {
	static constexpr std::size_t positionSize = 3;
	static constexpr std::size_t colorSize = 4;

	// x, y, z -> [0.0, 1.0]
	float position[positionSize];

	// RGBA -> [0, 255]
	ubyte color[colorSize];

	friend std::istream& operator>>(std::istream& is, Vertex& vertex) {
		std::string line{};
		std::getline(is, line);

		if (!is.eof()) {

			ltrim(line);
			if (line.front() == 'v') {
				std::istringstream posStream{ line.substr(line.find('v') + 1, line.find('c') - 1) };
				for (std::size_t i = 0; i < Vertex::positionSize; ++i) {
					posStream >> vertex.position[i];
				}
				if (posStream.fail()) {
					is.setstate(posStream.rdstate());
				}

				std::istringstream colStream{ line.substr(line.find('c') + 1) };
				ushort temp;
				for (std::size_t i = 0; i < Vertex::colorSize; ++i) {
					colStream >> temp;
					vertex.color[i] = static_cast<ubyte>(temp);
				}
				if (colStream.fail()) {
					is.setstate(colStream.rdstate());
				}
			}
			else {
				is.setstate(std::ios::failbit);
			}
		}
		else {
			is.setstate(std::ios::eofbit);
		}
		return is;
	}
};

