#ifndef NEW_OBJ_H
#define NEW_OBJ_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <QOpenGLWidget>
#include <renderer.h>
#include <editorController.h>

class Renderer;
class editorController;

class obj
{
public:
  GLuint texture;
  glm::vec2 position;
  glm::vec2 size;
  GLfloat rotate;
  glm::vec3 color;
  GLuint shader;
  GLuint VAO;

  int text_w, text_h;

  obj(Renderer *r, editorController *ec, const char *texture_path);
  ~obj();

  void render(QOpenGLFunctions_4_3_Core *f);
  void reload_shaders(QOpenGLFunctions_4_3_Core *f, flyweight<std::string> vertexId, flyweight<std::string> fragmentId);
  bool load_new_texture(const char *path, Renderer *r);
};

#endif //NEW_OBJ_H
