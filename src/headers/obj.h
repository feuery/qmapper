#ifndef OBJ_H
#define OBJ_H

#include <gl_apu.h>
#include <renderer.h>
#include<boost/flyweight.hpp>

class Renderer;

class immutable_obj{
public:

  GLuint vao_handle = -1;
  GLuint shader_handle;
  GLuint texture = -1;
  bool load_texture(QString& path, Renderer* r);

  int texture_width_px, texture_height_px;
  
  Point left_up;

  bool shader_loaded;
  
  immutable_obj(QOpenGLFunctions_4_3_Core *f);
  // ~immutable_obj();

  void render(QOpenGLFunctions_4_3_Core *f);

  Point3D GLLoc = {0.0f, 0.0f, 0.0f},
    newPXLoc = {0.0f, 0.0f, 0.0f},
    oldPXLoc = {0.0f, 0.0f, 0.0f};

  bool reload_shaders(QOpenGLFunctions_4_3_Core *f, boost::flyweights::flyweight<std::string> vertexShaderId, boost::flyweights::flyweight<std::string> fragmentShaderId);
  void setPixelLocation(Point3D point, int container_w, int container_h);
  Point3D getGLLocation();
  Point3D getPixelLocation();

};

#endif //OBJ_H
