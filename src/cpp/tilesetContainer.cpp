#include <tilesetContainer.h>

#include <QDebug>

tilesetContainer::tilesetContainer(Renderer *r, const char *tilesetPath): Tileset(), obj(r, editorController::instance, tilesetPath) {
  
}
