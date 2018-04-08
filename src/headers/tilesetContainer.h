#ifndef TILESETCONTAINER_H
#define TILESETCONTAINER_H

#include <tileset.h>
#include <renderable.h>
#include <new_obj.h>
#include <tuple>

class Tilesetcontainer: public Tileset, public Renderable {
public:

  std::vector<std::vector<std::string>> tiles;

  void setTileSize(int tilesetW, int tilesetH);
  
  int tiles_w, tiles_h; 	// dimensions of the tiles array
  Tilesetcontainer(Renderer *r, const char *tilesetPath);
  Tilesetcontainer();

  virtual void render(QOpenGLFunctions_4_3_Core *f) override;
  virtual void render(Renderer *parent) override;
  virtual void render() override;
  std::string getRenderId() const override;

  std::tuple<int, int> getTileCoordById(std::string id);

private:
  void load_texture_splitted(Renderer *p, const char *filename);
};

#endif //TILESETCONTAINER_H
