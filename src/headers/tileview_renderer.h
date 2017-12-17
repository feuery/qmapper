#ifndef TILEVIEW_RENDERER_H
#define TILEVIEW_RENDERER_H

#include <renderer.h>

class Renderer;

class tileview_renderer: public Renderer {
  Q_OBJECT
public:

  tileview_renderer();
  void setSelectedTile(int x, int y, obj *tileset_texture);
								      
protected slots:
  virtual void paintGL() override;
  virtual void initializeGL() override;
  
private:

  glm::vec2 size;
  glm::vec2 position, selectedTileCoord;
  GLuint shader, texture, VAO;

};

#endif //TILEVIEW_RENDERER_H
