#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <rootContainer.h>
#include <simplelistmodel.h>
#include <vector>
#include <mainwindow.h>

class MainWindow;

/* Holds the state of the editor. Will be passed everywhere as a reference */
class editorController{
public:
  void registerWindow(MainWindow *w);

  Rootcontainer document;
  Tilelistmodel documentTreeModel;
  Simplelistmodel *slm;

  QObject r;

  ~editorController();
  editorController();

private:

  MainWindow *w;

};

#endif //EDITORCONTROLLER_H
