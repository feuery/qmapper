#ifndef OBJ_H
#define OBJ_H

#include <texture.h>
#include <gl_apu.h>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLFunctions>

class immutable_obj{
public:
  GLuint texture;
  GLuint vao_handle;
  GLuint shader_handle;
  Point left_up;

  const char* vertex_shader_path;
  const char* fragment_shader_path;
  bool shader_loaded;
  int text_w, text_h;
  
  immutable_obj(QOpenGLFunctions_4_3_Core *f,
			     const char* texture_path,
			     const char* vertex_shader_path,
			     const char* fragment_shader_path,
			     Point left_up);
  ~immutable_obj();

  Rect getSize();
  void render(QOpenGLFunctions_4_3_Core *f);

  Point3D GLLoc = {0.0f, 0.0f, 0.0f},
    newPXLoc = {0.0f, 0.0f, 0.0f},
    oldPXLoc = {0.0f, 0.0f, 0.0f};

  bool reload_shaders(QOpenGLFunctions_4_3_Core *f);
  void setup_texture(QOpenGLFunctions_4_3_Core *f, const char* filename);
  void setPixelLocation(Point3D point, int container_w, int container_h);
  Point3D getGLLocation();
  Point3D getPixelLocation();
  
};

#endif //OBJ_H
