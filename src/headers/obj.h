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

  Point3D oldLoc = {0.0f, 0.0f, 0.0f}, newLoc = {0.0f, 0.0f, 0.0f};

  bool reload_shaders(QOpenGLFunctions_4_3_Core *f);
  void setup_texture(QOpenGLFunctions_4_3_Core *f, const char* filename);
  void setLocation(Point3D point);
  Point3D getLocation();
  
};

#endif //OBJ_H
