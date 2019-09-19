#include <colored_rect.h>
#include <editorController.h>
#include <iostream>

colored_rect::colored_rect(Renderer *r, int w, int h, QColor c):qi_copy(w, h, QImage::Format_ARGB32) {
  auto ec = editorController::instance;
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  text_w = w;
  text_h = h;

  qi_copy.fill(c);

  int r_w = r->width(),
    r_h =   r->height();
  prepare(f, r_w, r_h, ec);
  f->glGenTextures(1, &texture);
  f->glBindTexture(GL_TEXTURE_2D, texture);
  
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, qi_copy.bits());
  f->glGenerateMipmap(GL_TEXTURE_2D);
  f->glBindTexture(GL_TEXTURE_2D, 0);

  assert(size.x > 0.0f);
  assert(size.y > 0.0f);
}

void colored_rect::prepare(QOpenGLFunctions_4_3_Core *fns, GLfloat parentw, GLfloat parenth, editorController *ec)
{
  shader = createShader(fns, ec);  
  VAO = getVAO(fns, shader);
  doProjection(fns, parentw, parenth, shader);
  // std::cout << "Created VAO " << VAO;
  position = glm::vec2(0.0f, 0.0f);
  // printf("Text size: %d * %d\n", text_w, text_h);
  size = glm::vec2(static_cast<float>(text_w), static_cast<float>(text_h));
  rotate = 0.0f;
  color = glm::vec3(1.0f, 1.0f, 1.0f);
  opacity = 255;
}

colored_rect::~colored_rect() {
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  f->glDeleteVertexArrays(1, &VAO);
  f->glDeleteProgram(shader);
  f->glDeleteTextures(1, &texture);
  for(auto renderer: editorController::instance->renderers) {
    renderer->deleteOwnObject(id);
  }
}

void colored_rect::render(QOpenGLFunctions_4_3_Core *f) {
  render();
}

void colored_rect::render(Renderer *parent) {

}
void colored_rect::render()
{
  auto f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
  f->glUseProgram(shader);
  // qDebug() << "Position " << position.x << ", " << position.y;
  glm::mat4 model;
  model = glm::translate(model, glm::vec3(position, 0.0f));
  //Move pivot point to center
  // qDebug() << "Size: " << size.x << " * " << size.y;
  model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));

  //Rotate if needed
  // qDebug() << "Rotate: " << rotate;
  model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));

  //Move object
  model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

  //Scale object
  model = glm::scale(model, glm::vec3(size, 1.0f));

  f->glUniformMatrix4fv(f->glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
  f->glUniform3f(f->glGetUniformLocation(shader, "spriteColor"), color.x, color.y, color.z);

  auto op_loc = f->glGetUniformLocation(shader, "opacity");

  if(op_loc < 0) {
    puts("No opacity location found");
    throw "";
  }
  // qDebug() << "Opacity" << opacity;
  float op = (float)opacity / 255.0f;

  f->glUniform4f(op_loc, op, op, op, op);
  
  f->glActiveTexture(GL_TEXTURE0);
  f->glBindTexture(GL_TEXTURE_2D, texture);

  f->glBindVertexArray(VAO);
  f->glDrawArrays(GL_TRIANGLES, 0, 6);
  f->glBindVertexArray(0);

  GLenum err (f->glGetError());
 
  while(err!=GL_NO_ERROR) {
    std::string error;
 
    switch(err) {
    case GL_INVALID_OPERATION:      error="INVALID_OPERATION";      break;
    case GL_INVALID_ENUM:           error="INVALID_ENUM";           break;
    case GL_INVALID_VALUE:          error="INVALID_VALUE";          break;
    case GL_OUT_OF_MEMORY:          error="OUT_OF_MEMORY";          break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
    }
 
    printf("GL_%s\n", error.c_str());
    err=f->glGetError();
  }
}

std::string colored_rect::getRenderId() const{
  return id;
}
