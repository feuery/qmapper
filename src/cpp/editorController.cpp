#include <mapContainer.h>
#include <layerContainer.h>
#include <editorController.h>

editorController* editorController::instance;

// editorController* editorController::getInstance()
// {
//   if(!instance)
//     instance = new editorController();
//   return instance;
// }

void editorController::registerWindow(MainWindow *w)
{
  this->w = w;
}

editorController::editorController()
{

  if(instance) {
    puts("There already exists an editorController");
    throw "";
  }

  instance = this;
  
  for(int i = 0; i < 3; i++) {
    Mapcontainer *m = new Mapcontainer();
    m->setName(std::to_string(i)+"th map");
    m->parent(&document);
    for(int x = 0; x < 2; x++) {
      Layercontainer *l = new Layercontainer(10 + 10*x, 10 + 10*x);
      l->setName(std::to_string(x)+"th layer");
      l->set_parent(m);
      m->layers->push_back(l);
    }
    flyweight<std::string> id = m->getId();
    Propertierbase *b = m;
    (*document.registry)[id] = b;
  }

  documentTreeModel = new Tilelistmodel(&document);
}

editorController::~editorController()
{
  delete documentTreeModel;
}
