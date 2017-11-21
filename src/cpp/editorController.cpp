#include <propertierbase.h>
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

editorController::editorController(): indexOfChosenTileset(flyweight<std::string>(""))
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
  scr->setContents("#version 430 core\nlayout (location = 0) in vec4 inp; // <vec2 pos, vec2 texPos>\n\n//uniform vec4 loc;\n\nout vec2 TexCoord;\n\nuniform mat4 model;\nuniform mat4 projection;\n\nvoid main()\n{\n  gl_Position = projection * model * vec4(inp.xy, 0.0, 1.0);\n  TexCoord = inp.zw;\n}\n");
  (*document.registry)[scr->getId()] = scr;
  indexOfStdVertexShader = scr->getId();

  scr = new Script;
  scr->setScript_type(glsl);
  scr->setName("Standard fragment shader");
  scr->setNs("defaultShader");
  scr->setContents("#version 430 core\nin vec2 TexCoord;\nout vec4 color;\n\nuniform sampler2D image;\nuniform vec3 spriteColor;\n\nvoid main() {\n  vec4 texel = vec4(spriteColor, 1.0) * texture(image, TexCoord);\n  if(texel.a < 0.5) discard;\n\n  color = texel;\n}");
  (*document.registry)[scr->getId()] = scr;
  indexOfStdFragmentShader = scr->getId();
}

editorController::~editorController()
{
  delete documentTreeModel;
}

QOpenGLFunctions_4_3_Core* editorController::getGlFns()
{
  if(!ctx_provider) {
    qDebug() << "Can't get QOpenGLFunctions when provider is nil";
    throw "";
  }

  return ctx_provider->getGlFns();
}

void editorController::freeCtx()
{
  if(!ctx_provider) {
    qDebug() << "Can't get QOpenGLFunctions when provider is nil";
    throw "";
  }

  ctx_provider->freeCtx();
}
