#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <rootContainer.h>
#include <vector>
#include <mainwindow.h>

#include <textureContainer.h>
#include <renderer.h>
#include <tileview_renderer.h>

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

  flyweight<std::string> indexOfChosenTileset;
  flyweight<std::string> indexOfStdVertexShader;
  flyweight<std::string> indexOfStdFragmentShader, indexOfStdTileviewFragShader;

  int selectedTileX = 0, selectedTileY = 0;

  void setSelectedTile(int x, int y, Renderer *tilesetView, tileview_renderer *tileRenderer);
  
  /* BUT because guile_layer needs to access here too, we're forced to singleton this */
  static editorController *instance;

  static editorController* getInstance();

  Renderer* ctx_provider = nullptr;

  QOpenGLFunctions_4_3_Core* getGlFns();
  void freeCtx();
  
private:

  MainWindow *w;

};

#endif //EDITORCONTROLLER_H
