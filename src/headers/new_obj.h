#ifndef NEW_OBJ_H
#define NEW_OBJ_H

#include<boost/flyweight.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <QOpenGLWidget>
#include <renderer.h>
#include <renderable.h>

using namespace boost::flyweights;

GLuint CreateShaderProgram(QOpenGLFunctions_4_3_Core *f, GLuint vertex_shader, GLuint fragment_shader);
GLuint getVAO(QOpenGLFunctions_4_3_Core *f, GLfloat window_w, GLfloat window_h, GLuint shader);

class Renderer;
class editorController;

class obj: public Renderable
{
public:
  glm::vec2 position;
  glm::vec2 size;
  GLfloat rotate;
  glm::vec3 color;
  GLuint shader, texture;
  GLuint VAO;

  QImage copy;

  int text_w, text_h;

  int id; 

  static obj* make(Renderer *r, const char *texture_path, bool skipTexture = false);
  static obj* make(Renderer *parent, QImage img);

  ~obj();

  virtual void render(QOpenGLFunctions_4_3_Core *f);
  virtual void render(Renderer *parent) override;
  void reload_shaders(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> vertexId, flyweight<std::string> fragmentId);
  bool load_new_texture(const char *path, Renderer *r);

  int getRenderId() override;

private:

  obj(Renderer *r, const char *texture_path, bool skipTexture = false);
  obj(Renderer *r, QOpenGLFunctions_4_3_Core *f, QImage img);
  
  void prepare(QOpenGLFunctions_4_3_Core *fns, GLfloat parentw, GLfloat parenth, editorController *ec);
  void prepare(QOpenGLFunctions_4_3_Core *fns, Renderer *r, editorController *ec);
};

#else

class obj;

#endif //NEW_OBJ_H
