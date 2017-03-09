#include <QtDebug>
#include <obj.h>
#include <shaders.h>
#include <gl_apu.h>


immutable_obj::immutable_obj(QOpenGLFunctions_4_3_Core *f,
			     const char* texture_path,
			     const char* vertex_shader_path,
			     const char* fragment_shader_path,
			     Point left_up): vertex_shader_path(vertex_shader_path),
					     fragment_shader_path(fragment_shader_path),
					     shader_loaded(false)
{
  vao_handle = generateRectangle(f,
				 {left_up.x + 0.5f, left_up.y}, //oikee ylä
				 {left_up.x + 0.5f, left_up.y - 0.5f}, //oikee ala
				 {left_up.x, left_up.y - 0.5f}, // vasen ala
				 left_up //vasen ylä
				 );

  if(!reload_shaders(f))
    throw "Reloading shaders failed";
  else
    qDebug() << "Loading shaders succeded";

  // setLocation({0.0f, 0.0f, 0.0f});
  setup_texture(f, texture_path);
}

immutable_obj::~immutable_obj() {
  
}

void immutable_obj::setup_texture(QOpenGLFunctions_4_3_Core *f, const char* filename)
{
  if(strcmp(filename, "") == 0) {
    qDebug() << "Not loading texture's from an empty string";
    return;
  }
  if(!shader_loaded) return;
  
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char* img = SOIL_load_image(filename, &text_w, &text_h, 0, SOIL_LOAD_RGB);  
  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, text_w, text_h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
  f->glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(img);
  f->glBindTexture(GL_TEXTURE_2D, 0);
}

void immutable_obj::setLocation(Point3D p)
{
  // GLfloat v[] = {p.x, p.y, p.z, 1.0f};
  // auto loc_uniform = f->glGetUniformLocation(shader_handle, "loc");
  // f->glUniform4fv(loc_uniform, 4, v);
  // qDebug()<<"New Location is " << p.x << ", " << p.y << ", " << p.z;
  newLoc = p;
}

Point3D immutable_obj::getLocation()
{
  return newLoc;
  // GLfloat v[4];
  // auto loc_uniform = f->glGetUniformLocation(shader_handle, "loc");
  // f->glGetUniformfv(shader_handle, loc_uniform, v);
  // return {v[0], v[1], v[2]};
}

Rect immutable_obj::getSize()
{
  return {text_w, text_h};
}

void immutable_obj::render(QOpenGLFunctions_4_3_Core *f)
{
  // f->glBindTexture(GL_TEXTURE_2D, texture);
  f->glUseProgram (shader_handle);

  if(oldLoc != newLoc) {
    // qDebug()<< "Setting location from " << oldLoc << " to " << newLoc;
    oldLoc = newLoc;
    GLfloat v[] = {oldLoc.x, oldLoc.y, oldLoc.z, 0.0f};
    auto loc_uniform = f->glGetUniformLocation(shader_handle, "loc");
    f->glUniform4fv(loc_uniform, 1, v);
  }    

  f->glBindVertexArray(vao_handle);
  
  f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    printf("Shaders loaded at paths %s, %s", vertex_shader_path, fragment_shader_path);
    
    return true;
  }
  catch (...) {
    puts("Caught unknown exception at immutable_obj::reload_shaders()");
    return false;
  }
}
