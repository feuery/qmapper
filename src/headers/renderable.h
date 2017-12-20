#ifndef RENDERABLE_H
#define RENDERABLE_H

// C++'s type system sucks
class Renderable{
public:
  virtual void render(QOpenGLFunctions_4_3_Core *f) = 0;

};

#endif //RENDERABLE_H
