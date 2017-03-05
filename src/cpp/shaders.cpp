#include <QOpenGLFunctions_4_3_Core>

#include <shaders.h>
#include <files.h>
#include <string>
#include <cstring>
#include <QtDebug>

using namespace std;

GLuint LoadShaders(QOpenGLFunctions_4_3_Core *f, const char * vertex_file_path, SHADER_TYPE s) {
  
  std::string result = read_source(vertex_file_path);
  const char* c_haisee = result.c_str();

  const char* const* source = &c_haisee;
  
  GLuint vertexShader = f->glCreateShader(s == VERTEX? GL_VERTEX_SHADER: GL_FRAGMENT_SHADER);
  f->glShaderSource(vertexShader, 1, source, NULL);
  f->glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if(!success) {
    f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Loading shader %s failed: %s\n", vertex_file_path, infoLog);
  }
  else qDebug()<<"Compiled shader " << vertex_file_path;

  return vertexShader;
}

GLuint CreateShaderProgram(QOpenGLFunctions_4_3_Core *f,
			   GLuint vertex_shader,
			   GLuint fragment_shader) {

  GLuint shaderProgram = f->glCreateProgram();
  f->glAttachShader(shaderProgram, vertex_shader);
  f->glAttachShader(shaderProgram, fragment_shader);
  f->glLinkProgram(shaderProgram);
  GLint success;
  GLchar infolog[512];
  f->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if(!success) {
    f->glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
    printf("Linking shader program failed: %s\n", infolog);
    return -1;
  }

  return shaderProgram;
}
