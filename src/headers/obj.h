#ifndef OBJ_H
#define OBJ_H

#include <gl_apu.h>
#include<boost/flyweight.hpp>

class immutable_obj{
public:

  GLuint vao_handle;
  GLuint shader_handle;
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


 protected:
  virtual GLuint getTexture() = 0;
};

#endif //OBJ_H
