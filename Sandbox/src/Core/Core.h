#pragma once

#define ASSERT(x) if(!(x)) __debugbreak();
#define CALL(x) ClearError(); x; ASSERT(LogCall(#x, __FILE__, __LINE__))

void ClearError();

bool LogCall(const char*, const char*, int);