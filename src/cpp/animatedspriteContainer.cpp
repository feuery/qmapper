#include <animatedspriteContainer.h>

Animatedspritecontainer::Animatedspritecontainer() {
  sprites = new std::vector<Sprite*>;
}

Animatedspritecontainer::~Animatedspritecontainer() {
  delete sprites;
}

void Animatedspritecontainer::render(QOpenGLFunctions_4_3_Core *f){
  sprites->at(getCurrentframeid())->render(f);
}

void Animatedspritecontainer::render(Renderer *parent){
  sprites->at(getCurrentframeid())->render(parent);
}

int Animatedspritecontainer::getRenderId(){
  return rand(); 		// This doesn't reeeallly matter unless you're in obj
}

void Animatedspritecontainer::setX (int newX) {
  for(auto i = sprites->begin(); i != sprites->end(); i++) {
    (*i)->setX(newX);
  }
}
int Animatedspritecontainer::getX () {
  return (*sprites->begin())->getX();
}
void Animatedspritecontainer::setY (int newY) {
  for(auto i = sprites->begin(); i != sprites->end(); i++) {
    (*i)->setY(newY);
  }
}
int Animatedspritecontainer::getY () {
    return (*sprites->begin())->getY();
}

int Animatedspritecontainer::maxFrames () {
  return sprites->size();
}
void Animatedspritecontainer::advanceFrame () {
  int newId = getCurrentframeid() % maxFrames();
  setCurrentframeid(newId);
}
void Animatedspritecontainer::setAngle (float newangle) {
  for(auto i = sprites->begin(); i != sprites->end(); i++) {
    (*i)->setAngle(newangle);
  }
}
float Animatedspritecontainer::getAngle () {
  return (*sprites->begin())->getAngle();
}
