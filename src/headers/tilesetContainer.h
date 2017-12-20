#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>
#include <renderable.h>
#include <new_obj.h>

class tilesetContainer: public Tileset, public Renderable {
public:

  obj*** tiles;
  int tiles_w, tiles_h; 	// dimensions of the tiles array
  tilesetContainer(Renderer *r, const char *tilesetPath);

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;

private:
  void load_texture_splitted(QOpenGLFunctions_4_3_Core *f, const char *filename);
};

#endif //TILESETCONTAINER_H
