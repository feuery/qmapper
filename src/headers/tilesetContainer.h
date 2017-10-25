#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>

class tilesetContainer: public Tileset {
public:

  // tilesetContainer();
  // ~tilesetContainer();

  virtual void render (QOpenGLFunctions_4_3_Core *f) override;
private:

};

#endif //TILESETCONTAINER_H
