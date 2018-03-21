#include <spriteContainer.h>
#include <editorController.h>
#include <QFileInfo>

obj* Spritecontainer::getObject() const {
  return static_cast<obj*>(parent->owned_objects[renderId] );
}

Spritecontainer* Spritecontainer::make(Renderer *parent, const char *text_path) {
  Spritecontainer* sprite = new Spritecontainer(parent, text_path);
  QString p = text_path;
  QFileInfo finfo(p);
  sprite->setName(("Sprite: " + finfo.fileName().toStdString()).c_str());

  (*editorController::instance->document.getSprites())[sprite->getId()] = sprite;
  
  return sprite;
}

Spritecontainer::Spritecontainer() {}

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
    newXf = newX;

  obj* o = getObject();
  
  o->position = glm::vec2(newXf, y);

  handleEvents("X");
}
int Spritecontainer::getX () const  {
  return ceil(getObject()->position.x);
  
}
void Spritecontainer::setY (int newY)  {
  float x = getObject()->position.x,
    newYf = newY;
  
  getObject()->position = glm::vec2(x, newYf);

  handleEvents("Y");
}
int Spritecontainer::getY () const {
  return ceil(getObject()->position.y);
}
void Spritecontainer::setAngle (float newangle)  {
  getObject()->rotate = newangle;
  handleEvents("Angle");
}
float Spritecontainer::getAngle () const  {
  return getObject()->rotate;
}


void Spritecontainer::render(QOpenGLFunctions_4_3_Core *f)  {
  obj* o = getObject();
  o->render(f);
}

void Spritecontainer::render()  {
  obj* o = getObject();
  o->render();
}

void Spritecontainer::render(Renderer *parent)  {
  getObject()->render(parent);
}
int Spritecontainer::getRenderId()  {
  return getObject()->getRenderId();
}
