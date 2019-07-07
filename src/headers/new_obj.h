#ifndef NEW_OBJ_H 
#define NEW_OBJ_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <QOpenGLWidget>
#include <renderer.h>
#include <renderable.h>

class Renderer;
class editorController;

GLuint CreateShaderProgram(QOpenGLFunctions_4_3_Core *f, GLuint vertex_shader, GLuint fragment_shader);
GLuint createShader(QOpenGLFunctions_4_3_Core *f, editorController *ec);
void doProjection(QOpenGLFunctions_4_3_Core *f, GLfloat window_w, GLfloat window_h, GLuint shader);
GLuint getVAO(QOpenGLFunctions_4_3_Core *f, GLuint shader);

class obj: public Renderable
{
public:
  glm::vec2 position;
  glm::vec2 size;
  GLfloat rotate;
  glm::vec3 color;
  GLuint shader, texture;
  GLuint VAO, VBO;

  Renderer *parent;

  QImage qi_copy;

  int text_w, text_h;

  std::string id;

  unsigned char opacity;

  static obj* make(Renderer *r, const char *texture_path, bool skipTexture = false, std::string id = std::to_string(rand()));
  static obj* make(Renderer *parent, QImage img, std::string id = std::to_string(rand()));
  static std::string make(QImage img, std::string id = std::to_string(rand()));

  static std::string make(int x, int y, int w, int h, QColor& c);

  // Cached for copying purposes:
  const char *text_path;
  bool skipTexture;
  bool qi_copied = false;

  ~obj();

  virtual void render(QOpenGLFunctions_4_3_Core *f);
  virtual void render(Renderer *parent) override;
  virtual void render() override;
  void reload_shaders(QOpenGLFunctions_4_3_Core *f, std::string vertexId, std::string fragmentId);
  bool load_new_texture(const char *path, Renderer *r);

  std::string getRenderId() const override;

  obj* subObj = nullptr;

  Renderable* copyRenderable();

protected:
  obj(Renderer *r, const char *texture_path, bool skipTexture = false);
  obj(Renderer *r, QOpenGLFunctions_4_3_Core *f, QImage img);

private:
  void prepare(QOpenGLFunctions_4_3_Core *fns, GLfloat parentw, GLfloat parenth, editorController *ec);
  void prepare(QOpenGLFunctions_4_3_Core *fns, Renderer *r, editorController *ec);

  GLuint load_texture(QOpenGLFunctions_4_3_Core *f, const char *filename, int *text_w, int *text_h);
};

#else

class obj;

#endif //NEW_OBJ_H
