#pragma once

class IBindable {
public:
	virtual ~IBindable() = 0;
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
};