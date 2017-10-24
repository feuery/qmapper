#include <mapContainer.h>
#include <layerContainer.h>
#include <editorController.h>

#include <script.h>

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

  Script *scr = new Script;
  scr->setScript_type(glsl);
  scr->setNs("defaultVertex");
  scr->setName("Standard vertex shader");
  scr->setContents("#version 430 core\nlayout (location = 0) in vec3 position;\nlayout (location = 1) in vec3 color;\nlayout (location = 2) in vec2 texCoord;\n\nuniform vec4 loc;\n\nout vec3 ourColor;\nout vec2 TexCoord;\n\nvoid main()\n{\n  gl_Position = vec4(position, 1.0f) + loc;\n  ourColor = color;\n  TexCoord = texCoord;\n}\n");
  (*document.registry)[scr->getId()] = scr;

  scr = new Script;
  scr->setScript_type(glsl);
  scr->setName("Standard fragment shader");
  scr->setNs("defaultShader");
  scr->setContents("#version 430 core\nin vec3 ourColor;\nin vec2 TexCoord;\n\nout vec4 color;\n\nuniform sampler2D ourTexture;\n\nvoid main() {\n  color = texture(ourTexture, TexCoord);\n}");

  (*document.registry)[scr->getId()] = scr;
}

editorController::~editorController()
{
  delete documentTreeModel;
}
