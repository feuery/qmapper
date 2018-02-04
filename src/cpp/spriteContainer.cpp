#include <spriteContainer.h>

Spritecontainer* Spritecontainer::make(Renderer *parent, const char *text_path) {
  return new Spritecontainer(parent, text_path);
}

Spritecontainer::Spritecontainer( Renderer *parent, const char *text_path): obj(parent, text_path) {
  
}

void Spritecontainer::setX (int newX)
{
  float y = position.y,
    newXf = newX/parent->width();
  
  position = glm::vec2(newXf, y);

}
int Spritecontainer::getX ()  {
  float x = position.x;
  return static_cast<int>(x * parent->width());
}
void Spritecontainer::setY (int newY)  {
  float x = position.x,
    newYf = newY/parent->height();
  
  position = glm::vec2(x, newYf);
}
int Spritecontainer::getY () {
  float y = position.y;
  return static_cast<int>(y * parent->height());
}
void Spritecontainer::setAngle (float newangle)  {
  rotate = newangle;
}
float Spritecontainer::getAngle ()  {
  return rotate;
}
