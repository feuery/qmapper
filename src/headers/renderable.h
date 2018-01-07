#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <renderer.h>
// C++'s type system sucks
class Renderable{
public:
  virtual void render(QOpenGLFunctions_4_3_Core *f) = 0;
  virtual void render(Renderer *parent) = 0;
  virtual int getRenderId() = 0;
};
#else
class Renderable;

#endif //RENDERABLE_H
