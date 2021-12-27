#pragma once

#include <iostream>
#include <string>
#include <utility>

class OStreamDelegate {
public:
	OStreamDelegate(std::ostream& outStream) :
		m_OutStream{ outStream } {
	}
	~OStreamDelegate() {
		m_OutStream << std::endl;
	}

	template <typename T>
	OStreamDelegate& operator<<(T&& output) {
		m_OutStream << std::forward<T>(output);
		return *this;
	}

	OStreamDelegate(const OStreamDelegate&) = delete;
	OStreamDelegate& operator=(const OStreamDelegate&) = delete;

private:
	std::ostream& m_OutStream;
};
