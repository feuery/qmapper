#ifndef SHADERS_H
#define SHADERS_H

/* #include <main.h> */

enum SHADER_TYPE {
  VERTEX,
  FRAGMENT
};

GLuint CreateShaderProgram(QOpenGLFunctions_4_3_Core *f,
			   GLuint vertex_shader,
			   GLuint fragment_shader);
GLuint LoadShaders(QOpenGLFunctions_4_3_Core *f, const char * vertex_file_path, SHADER_TYPE s);

#endif //SHADERS_H

