#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <map.h>
#include <vector>
#include <mainwindow.h>

class MainWindow;

/* Holds the state of the editor. Will be passed everywhere as a reference */
class editorController{
public:
  map Map;

  void registerWindow(MainWindow *w);

private:

  MainWindow *w;

};

#endif //EDITORCONTROLLER_H
