#include <editorController.h>

void editorController::registerWindow(MainWindow *w)
{
  this->w = w;
}

editorController::editorController(): documentTreeModel(&document) { }