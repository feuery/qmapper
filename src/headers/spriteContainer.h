#ifndef SPRITECONTAINER_H
#define SPRITECONTAINER_H

#include <sprite.h>

class Spritecontainer: public Sprite, public Renderable {
public:
  static Spritecontainer* make(Renderer *parent, const char *text_path);

  ~Spritecontainer();
  Spritecontainer();

  void setX (int newX) override;
  int getX () const override;
  void setY (int newY) override;
  int getY () const override;
  void setAngle (float newangle) override;
  float getAngle () const override;

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render() override;
  void render(Renderer *parent) override;
  std::string getRenderId() const override;

  Spritecontainer(Renderer *parent, QImage img);

private:
  Spritecontainer(Renderer *parent, const char *text_path);

  Renderer *parent;
  obj* getObject() const override;
};
#else
class Spritecontainer;
#endif //SPRITECONTAINER_H
