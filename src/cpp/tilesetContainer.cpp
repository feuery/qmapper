#include <tilesetContainer.h>

#include <QDebug>
#include <gl_apu.h>

// void tilesetContainer::render (QOpenGLFunctions_4_3_Core *f)
// {
//   qDebug() << "Rendering tileset";
// }

tilesetContainer::tilesetContainer(QOpenGLFunctions_4_3_Core *f): Tileset(), immutable_obj(f) {
  
}

// tilesetContainer::~tilesetContainer() {
//   qDebug() << "At ~tilesetContainer";
// }

// GLuint tilesetContainer::getTexture()
// {
//   return texture;
// }
