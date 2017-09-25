#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <rootContainer.h>
#include <vector>
#include <mainwindow.h>

#include <textureContainer.h>

class MainWindow;

/* Holds the state of the editor. Will be passed everywhere as a reference */
class editorController{
public:
  void registerWindow(MainWindow *w);

  Rootcontainer document;
  Tilelistmodel *documentTreeModel;

  QObject r;

  ~editorController();
  editorController();
  
  /* BUT because guile_layer needs to access here too, we're forced to singleton this */
  static editorController *instance;

  static editorController* getInstance();
private:

  MainWindow *w;

};

#endif //EDITORCONTROLLER_H
