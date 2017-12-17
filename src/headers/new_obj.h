#ifndef NEW_OBJ_H
#define NEW_OBJ_H

#include<boost/flyweight.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <QOpenGLWidget>
#include <renderer.h>

using namespace boost::flyweights;

GLuint CreateShaderProgram(QOpenGLFunctions_4_3_Core *f, GLuint vertex_shader, GLuint fragment_shader);
GLuint getVAO(QOpenGLFunctions_4_3_Core *f, GLfloat window_w, GLfloat window_h, GLuint shader);

class Renderer;

class obj
{
public:
  glm::vec2 position;
  glm::vec2 size;
  GLfloat rotate;
  glm::vec3 color;
  GLuint shader, texture;
  GLuint VAO;

  int text_w, text_h;

  obj(Renderer *r, const char *texture_path, bool skipTexture = false);
  ~obj();

  virtual void render(QOpenGLFunctions_4_3_Core *f);
  void reload_shaders(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> vertexId, flyweight<std::string> fragmentId);
  bool load_new_texture(const char *path, Renderer *r);
};

#endif //NEW_OBJ_H
