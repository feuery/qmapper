#ifndef COLORED_RECT_H 
#define COLORED_RECT_H

#include <new_obj.h>

class colored_rect: public Renderable {
public:
  glm::vec2 position;
  glm::vec2 size;
  GLfloat rotate;
  glm::vec3 color;
  GLuint shader, texture;
  GLuint VAO;

  Renderer *parent;

  int text_w, text_h;

  std::string id;

  unsigned char opacity;
  QImage qi_copy;

  colored_rect(Renderer *r, int w, int h, QColor color);
  ~colored_rect();

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render(Renderer *parent) override;
  virtual void render() override;

  std::string getRenderId() const override;

protected:
private:
  void prepare(QOpenGLFunctions_4_3_Core *fns, GLfloat parentw, GLfloat parenth, editorController *ec);
};


#endif //COLORED_RECT_H
