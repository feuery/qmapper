#include <QtDebug>
#include <obj.h>
#include <shaders.h>
#include <gl_apu.h>


immutable_obj::immutable_obj(QOpenGLFunctions_4_3_Core *f,
			     const char* texture_path,
			     const char* vertex_shader_path,
			     const char* fragment_shader_path): vertex_shader_path(vertex_shader_path),
					     fragment_shader_path(fragment_shader_path),
					     shader_loaded(false)
{
  vao_handle = generateRectangle(f);

  if(!reload_shaders(f))
    throw "Reloading shaders failed";
  else
    qDebug() << "Loading shaders succeded";

}

immutable_obj::~immutable_obj() {
  
}

void immutable_obj::setup_texture(QOpenGLFunctions_4_3_Core *f, const char* filename)
{
  if(strcmp(filename, "") == 0) {
    qDebug() << "Not loading texture's from an empty string";
    return;
  }
  if(!shader_loaded) { qDebug()<<"Shader isn't loaded?"; return; }

  
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);

  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* img = SOIL_load_image(filename, &text_w, &text_h, 0, SOIL_LOAD_AUTO);
  if(img) {
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, text_w, text_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    f->glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(img);
    f->glBindTexture(GL_TEXTURE_2D, 0);

    return;
  }
}

void immutable_obj::setPixelLocation(Point3D p, int container_w, int container_h)
{
  newPXLoc = p;
  float x = p.x,
    y = p.y;
  int w = container_w,
    h = container_h;
  GLLoc = {(x/w) - 0.5f, -(y/h) + 0.5f, 0.0};
}

Point3D immutable_obj::getPixelLocation()
{
  return newPXLoc;
}

Point3D immutable_obj::getGLLocation()
{
  return GLLoc;
}

Rect immutable_obj::getSize()
{
  return {text_w, text_h};
}

void immutable_obj::render(QOpenGLFunctions_4_3_Core *f)
{
  f->glUseProgram (shader_handle);

  if(oldPXLoc != newPXLoc) {
    oldPXLoc = newPXLoc;
    GLfloat v[] = {GLLoc.x, GLLoc.y, GLLoc.z, 0.0f};
    auto loc_uniform = f->glGetUniformLocation(shader_handle, "loc");
    f->glUniform4fv(loc_uniform, 1, v);
  }

  f->glActiveTexture(GL_TEXTURE0);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  f->glUniform1i(f->glGetUniformLocation(shader_handle, "ourTexture"), 0);

  f->glBindVertexArray(vao_handle);
  
  f->glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);
  f->glBindVertexArray(0);
}

bool immutable_obj::reload_shaders(QOpenGLFunctions_4_3_Core *f)
{
  try {
    if(shader_loaded)
      f->glDeleteProgram(shader_handle);

    if(!vertex_shader_path || !fragment_shader_path) return true;
  
    auto vShader = LoadShaders(f, vertex_shader_path, VERTEX),
      fShader = LoadShaders(f, fragment_shader_path, FRAGMENT);

    shader_handle = CreateShaderProgram(f, vShader, fShader);

    f->glDeleteShader(vShader);
    f->glDeleteShader(fShader);
    
    return shader_loaded = true;
  }
  catch (...) {
    puts("Caught unknown exception at immutable_obj::reload_shaders()");
    return shader_loaded = false;
  }
}
