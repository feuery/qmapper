#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <rootContainer.h>
#include <vector>
#include <mainwindow.h>

#include <tilesetContainer.h>
#include <renderer.h>
#include <tileview_renderer.h>

#include <tile.h>
#include <tool.h>

class MainWindow;
class Renderer;

/* Holds the state of the editor. Will be passed everywhere as a reference */
class editorController{
public:
  void registerWindow(MainWindow *w);

  Rootcontainer document;
  Tilelistmodel *documentTreeModel;

  QObject r;

  ~editorController();
  editorController();

  void populateMaps();

  flyweight<std::string> indexOfChosenTileset;
  flyweight<std::string> indexOfChosenMap;
  int indexOfChosenLayer;

  flyweight<std::string> indexOfStdVertexShader;
  flyweight<std::string> indexOfStdFragmentShader, indexOfStdTileviewFragShader;
  std::vector<Renderer*> renderers;

  int selectedTileX = 0, selectedTileY = 0;
  Tile selectedTileData;

  void setSelectedTile(int x, int y, Renderer *tilesetView, tileview_renderer *tileRenderer);
  void setTileAt(int x, int y);
  void setTileRotation(int x, int y, int deg_angl);
  void rotateTile90Deg(int x, int y);

  void loadSprite(const char *path);

  tilesetContainer *firstLoadedTileset = nullptr;
  std::vector<Tile*>* tiles = new std::vector<Tile*>;

  // Goddammit
  Renderer *map_view;
  Tool *t;

  bool renderingEnabled = true;
  
  
  /* BUT because guile_layer needs to access here too, we're forced to singleton this */
  static editorController *instance;

  static editorController* getInstance();

  // Stuff between here and private: probably needs removing
  Renderer* ctx_provider = nullptr;

  QOpenGLFunctions_4_3_Core* getGlFns();
  void freeCtx();
  

  MainWindow *w;

};

#endif //EDITORCONTROLLER_H
