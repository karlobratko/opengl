#pragma once

#include <iostream>
#include <string>
#include <utility>

class StreamDelegate {
public:
	StreamDelegate(std::ostream& os) :
		m_OutStream{ os } {
	}
	~StreamDelegate() {
		m_OutStream << std::endl;
	}

	template <typename T>
	StreamDelegate& operator<<(T&& output) {
		m_OutStream << std::forward<T>(output);
		return *this;
	}

	StreamDelegate(const StreamDelegate&) = delete;
	StreamDelegate& operator=(const StreamDelegate&) = delete;

	StreamDelegate(StreamDelegate&&) = default;
	StreamDelegate& operator=(StreamDelegate&&) = default;

private:
	std::ostream& m_OutStream;
};

class Logger {
public:
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	static Logger& Instance(std::ostream& os) {
		static Logger instance{ os };
		return instance;
	}

	StreamDelegate Log() {
		return StreamDelegate(m_OutStream);
	}

private:
	Logger(std::ostream& os) :
		m_OutStream{ os } {
	}

	std::ostream& m_OutStream;
};