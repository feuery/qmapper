#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>

class tilesetContainer: public Tileset {
public:

  // tilesetContainer();
  // ~tilesetContainer();

  virtual void render (QOpenGLFunctions_4_3_Core *f) override;
  virtual bool load_texture(QString& path, Renderer* r) override;
private:

};

#endif //TILESETCONTAINER_H
