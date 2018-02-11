#include <spriteContainer.h>
#include <editorController.h>
#include <QFileInfo>

obj* Spritecontainer::getObject() {
  return static_cast<obj*>(parent->owned_objects[renderId] );
}

Spritecontainer* Spritecontainer::make(Renderer *parent, const char *text_path) {
  Spritecontainer* sprite = new Spritecontainer(parent, text_path);
  QString p = text_path;
  QFileInfo finfo(p);
  sprite->setName(("Sprite: " + finfo.fileName().toStdString()).c_str());

  editorController::instance->document.doRegister("Sprite", sprite->getId(), sprite);
  
  return sprite;
}

Spritecontainer::Spritecontainer( Renderer *parent, const char *text_path): Sprite(), parent(parent) {
  // Such memory leak. References to these are kept under Renderers
  obj* obj = obj::make(parent, text_path);
  renderId = obj->getRenderId();
  setX(0);
  setY(0);
}

Spritecontainer::Spritecontainer( Renderer *parent, QImage img): Sprite(), parent(parent) {
  // Such memory leak. References to these are kept under Renderers
  obj* obj = obj::make(parent, img);
  renderId = obj->getRenderId();
  setX(0);
  setY(0);
}

Spritecontainer::~Spritecontainer() {
}

void Spritecontainer::setX (int newX)
{
  float y = getObject()->position.y,
    newXf = newX/getObject()->parent->width();
  
  getObject()->position = glm::vec2(newXf, y);

}
int Spritecontainer::getX ()  {
  float x = getObject()->position.x;
  return static_cast<int>(x * getObject()->parent->width());
}
void Spritecontainer::setY (int newY)  {
  float x = getObject()->position.x,
    newYf = newY/getObject()->parent->height();
  
  getObject()->position = glm::vec2(x, newYf);
}
int Spritecontainer::getY () {
  float y = getObject()->position.y;
  return static_cast<int>(y * getObject()->parent->height());
}
void Spritecontainer::setAngle (float newangle)  {
  getObject()->rotate = newangle;
}
float Spritecontainer::getAngle ()  {
  return getObject()->rotate;
}


void Spritecontainer::render(QOpenGLFunctions_4_3_Core *f)  {
  getObject()->render(f);
}
void Spritecontainer::render(Renderer *parent)  {
  getObject()->render(parent);
}
int Spritecontainer::getRenderId()  {
  return getObject()->getRenderId();
}
