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
  int getX () const override;
  void setY (int newY);
  int getY () const override;
  int maxFrames ();
  void advanceFrame ();
  void setAngle (float newangle);
  float getAngle () const override;;

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render(Renderer *parent) override;
  virtual void render() override;
  
  virtual int getRenderId() override;

  time_t last_changed;

  static void make(Renderer *parent, const char *spriteSheetPath, int frameCount, int frameLifeTime);

private:
  QImage root;
  void advanceFrameIfNeeded();

};

#endif //ANIMATEDSPRITECONTAINER_H
