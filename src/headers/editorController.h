#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <tilelistmodel.h>
#include <vector>
#include <mainwindow.h>

#include <renderer.h>
#include <tileview_renderer.h>

#include <tool.h>

#include <engine.h>
#include <libzippp.h>

class MainWindow;
class Renderer;

/* Holds the state of the editor. Will be passed everywhere as a reference */
class editorController{
public:
  void registerWindow(MainWindow *w);

  cl_object document;
  Tilelistmodel *documentTreeModel;

  QObject r;

  ~editorController();
  editorController();

  void populateMaps();
  
  std::vector<Renderer*> renderers;

  void clearDrawQueues();

  bool loaded = false;

  Engine *e;
  std::unordered_map<Qt::Key, bool> keyMap;
  int mouseX, mouseY;

  int selectedTileX = 0, selectedTileY = 0;
  cl_object selectedTileData;

  void setSelectedTile(int x, int y, Renderer *tilesetView, tileview_renderer *tileRenderer);
  void setTileAt(int x, int y);
  void setTileRotation(int x, int y, int deg_angl);
  void rotateTile90Deg(int x, int y);

  void loadSprite(const char *path);
  void loadAnimation(const char *path, int frameCount, int frameLifetime = 25);

  // Goddammit
  Renderer *map_view, *tilesetView;
  Tool *t;

  bool renderingEnabled = true;
  
  
  /* BUT because guile_layer needs to access here too, we're forced to singleton this */
  static editorController *instance;

  void saveTo(QString filename);
  void loadFrom(QString filename);

  MainWindow *w;

private:
  void dumpTextures(libzippp::ZipArchive &arch);

};

#endif //EDITORCONTROLLER_H
