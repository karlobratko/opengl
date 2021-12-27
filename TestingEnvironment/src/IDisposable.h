#pragma once

class IDisposable {
public:
	virtual ~IDisposable() = 0;
	virtual void Dispose() = 0;
};