
#include <QtDebug>
#include <obj.h>
#include <shaders.h>
#include <gl_apu.h>
#include <editorController.h>
#include <script.h>

#include <SOIL/SOIL.h>
#include <qopengl.h>

using namespace boost::flyweights;

immutable_obj::immutable_obj(QOpenGLFunctions_4_3_Core *f): shader_loaded(false)
{
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

void immutable_obj::render(QOpenGLFunctions_4_3_Core *f)
{
  if(!shader_loaded) {
    qDebug() << "Shader isn't loaded";
    return;
  }

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

GLuint fetchShader(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> id, SHADER_TYPE s) {
  std::string result = toScript(editorController::instance->document.registry->at(id))->getContents();
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
    printf("Loading shader %s failed: %s\nf", id.get().c_str(), infoLog);
  }
  else qDebug()<<"Compiled shader " << id.get().c_str();

  return vertexShader;
}

bool immutable_obj::reload_shaders(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> vertexShaderId, flyweight<std::string> fragmentShaderId)
{
  try {
    if(shader_loaded)
      f->glDeleteProgram(shader_handle);
  
    auto vShader = fetchShader(f, vertexShaderId, VERTEX);
    auto fShader = fetchShader(f, fragmentShaderId, FRAGMENT);

    shader_handle = CreateShaderProgram(f, vShader, fShader);

    f->glDeleteShader(vShader);
    f->glDeleteShader(fShader);

    shader_loaded = true;
    qDebug() << "Shader is loaded";
  }
  catch (...) {
    puts("Caught unknown exception at immutable_obj::reload_shaders()");
    return shader_loaded = false;
  }
}

bool immutable_obj::load_texture(QString& path, Renderer* r)
{
  texture = r->load_texture(path.toStdString().c_str(), &texture_width_px, &texture_height_px);
  vao_handle = generateRectangle(r->getGlFns(), texture_width_px, texture_height_px, r->width(), r->height());
  r->freeCtx();
  return texture > 0;
}
