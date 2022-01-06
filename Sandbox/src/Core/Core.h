#pragma once

#define ASSERT(x) if(!(x)) __debugbreak();
#define CALL(x) ClearError(); x; ASSERT(LogCall(#x, __FILE__, __LINE__))

#define RGBA(x) (x >> 24 & 0xff) / 255.0f, (x >> 16 & 0xff) / 255.0f, (x >> 8 & 0xff) / 255.0f, (x >> 0 & 0xff) / 255.0f

#define RAD(x) x * M_PI / 180.0f;
#define DEG(x) x * 180.0f / M_PI;

void ClearError();

bool LogCall(const char*, const char*, int);