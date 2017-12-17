#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>
#include <new_obj.h>

class tilesetContainer: public Tileset, public obj {
public:

  QVector<QVector<GLuint>> textures;
  tilesetContainer(Renderer *r, const char *tilesetPath);

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
};

#endif //TILESETCONTAINER_H
