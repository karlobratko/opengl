#pragma once

#include "OStreamDelegate.h"

class Logger {
public:
	static Logger& Instance(std::ostream& os) {
		static Logger instance{ os };
		return instance;
	}

	OStreamDelegate Log() {
		return OStreamDelegate{ m_OutStream };
	}

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

private:
	Logger(std::ostream& os) :
		m_OutStream{ os } {
	}

	std::ostream& m_OutStream;
};