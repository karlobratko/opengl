#include "gl_utils.h"

#include <GL/glew.h>
#include <stdio.h>

void ClearError() { while (glGetError() != GL_NO_ERROR); }
bool LogCall(const char* func, const char* file, int line) {
	while (unsigned int error = glGetError()) {
		if (error != GL_NO_ERROR) {
			fprintf(stderr, u8"[OpenGL::ERR] (err: %d)\nassertion failed: %s, file %s, line %d", error, func, file, line);
			return false;
		}
	}
	return true;
}