#include <mapContainer.h>
#include <layerContainer.h>
#include <editorController.h>

void editorController::registerWindow(MainWindow *w)
{
  this->w = w;
}

editorController::editorController(): documentTreeModel(&document)
{
  for(int i = 0; i < 3; i++) {
    Mapcontainer *m = new Mapcontainer();
    m->parent(&document);
    for(int x = 0; x < 2; x++) {
      Layercontainer *l = new Layercontainer(10 + 10*x, 10 + 10*x);
      l->set_parent(m);
      m->layers->push_back(l);
    }
    document.all_maps->push_back(m);
  }

  r.setObjectName("I am root");
  QObject *child;
  QObject *foo = new QObject(&r);
  foo->setObjectName("foo");
  child = new QObject(foo);
  child->setObjectName("Mark");
  child = new QObject(foo);
  child->setObjectName("Lollo");
  child = new QObject(foo);
  child->setObjectName("Pollo");
  slm = new Simplelistmodel(&r);
}

editorController::~editorController()
{
  delete slm;
}
