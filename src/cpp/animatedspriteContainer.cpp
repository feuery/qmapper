#include <animatedspriteContainer.h>
#include <QDebug>
#include <QFileInfo>
#include <editorController.h>

int MsTime() {
  return time(nullptr) * 1000;
}

Animatedspritecontainer::Animatedspritecontainer(Renderer *parent, const char *spriteSheetPath, int frameCount, int frameLifeTime): last_changed(MsTime()) {
  sprites = new std::vector<Sprite*>;
  root.load(QString(spriteSheetPath));
  int frameW = root.width() / frameCount,
    frameH = root.height();
  
  for(int i = 0; i < frameCount; i++) {    
    sprites->push_back(new Spritecontainer(parent, root.copy( i * frameW, 0, frameW, frameH)));
  }

  setMsperframe(frameLifeTime);

  QString p = spriteSheetPath;
  QFileInfo finfo(p);
  setName(("Animation: " + finfo.fileName().toStdString()).c_str());
  editorController::instance->document.doRegister("AnimatedSprite", getId(), this);
}

Animatedspritecontainer::~Animatedspritecontainer() {
  delete sprites;
}

void Animatedspritecontainer::make(Renderer *parent, const char *spriteSheetPath, int frameCount, int frameLifeTime) {
  new Animatedspritecontainer(parent, spriteSheetPath, frameCount, frameLifeTime);
}

void Animatedspritecontainer::advanceFrameIfNeeded() {
  if(getAnimationplaying() &&
     // This overflows like hell
     // But despite the sign, it works as expected
     // If this breaks, difftime might be a useful replacement for this expression
     MsTime() > last_changed + getMsperframe()) {
    advanceFrame();
    last_changed = MsTime();
  }
}

void Animatedspritecontainer::render(QOpenGLFunctions_4_3_Core *f){
  advanceFrameIfNeeded();
  auto sprite = 
    sprites->at(getCurrentframeid());
  if(sprite) sprite->render(f);
  else {
    qDebug() << "Trying to render a null sprite at Animatedspritecontainer::render(QOpenGLFunctions_4_3_Core*)";
    advanceFrame();
  }
}

void Animatedspritecontainer::render(Renderer *parent){
  advanceFrameIfNeeded();
  sprites->at(getCurrentframeid())->render(parent);
}

int Animatedspritecontainer::getRenderId(){
  qDebug() << "Don't call Animatedspritecontainer::getRenderId()";
  throw "Don't call Animatedspritecontainer::getRenderId()";
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
  int newId = (getCurrentframeid() + 1) % maxFrames();
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
