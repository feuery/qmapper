#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>
#include <obj.h>

class tilesetContainer: public Tileset, public immutable_obj {
public:

  tilesetContainer(QOpenGLFunctions_4_3_Core *f);
  // ~tilesetContainer();

  // virtual void render (QOpenGLFunctions_4_3_Core *f) override;
  virtual bool load_texture(QString& path, Renderer* r) override;
protected:
  virtual GLuint getTexture() override;
private:

};

#endif //TILESETCONTAINER_H
