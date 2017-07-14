#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <rootContainer.h>
#include <vector>
#include <mainwindow.h>

class MainWindow;

/* Holds the state of the editor. Will be passed everywhere as a reference */
class editorController{
public:
  map Map;

  void registerWindow(MainWindow *w);

  Rootcontainer document;

private:

  MainWindow *w;

};

#endif //EDITORCONTROLLER_H
