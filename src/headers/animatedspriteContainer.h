#ifndef ANIMATEDSPRITECONTAINER_H
#define ANIMATEDSPRITECONTAINER_H

#include <animatedSprite.h>
#include <spriteContainer.h>
#include <renderable.h>
#include <ctime>

class Animatedspritecontainer: public animatedsprite, public Renderable{
public:

  Animatedspritecontainer(Renderer *parent, const char *spriteSheetPath, int frameCount, int frameLifeTime);
  ~Animatedspritecontainer();

  void setX (int newX);
  int getX ();
  void setY (int newY);
  int getY ();
  int maxFrames ();
  void advanceFrame ();
  void setAngle (float newangle);
  float getAngle ();

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render(Renderer *parent) override;
  virtual int getRenderId() override;

  time_t last_changed;

  static void make(Renderer *parent, const char *spriteSheetPath, int frameCount, int frameLifeTime);

private:

  void advanceFrameIfNeeded();

};

#endif //ANIMATEDSPRITECONTAINER_H
