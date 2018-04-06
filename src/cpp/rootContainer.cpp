#include <tilelistmodel.h>
#include<rootContainer.h>
#include <script.h>
#include <QDebug>

Rootcontainer::Rootcontainer(): root()
{
  setAnimatedsprites(new std::map<std::string, animatedsprite*>);
  setLayers(new std::map<std::string, Layer*>);
  setMaps(new std::map<std::string, Map*>);
  setScripts(new std::map<std::string, Script*>);
  setSprites(new std::map<std::string, Sprite*>);
  setTiles(new std::map<std::string, Tile*>);
  setTilesets(new std::map<std::string, Tileset*>);
}

Rootcontainer::~Rootcontainer()
{
  // delete getRegistry();
}

either<scriptTypes, std::string> Rootcontainer::findNs(std::string ns)
{
  std::vector<Script*> scripts;

  either<scriptTypes, std::string> result;

  for(auto iter = getScripts()->begin(); iter != getScripts()->end(); iter++) {
    Script *s = toScript(iter->second);
      if(s->getNs() == ns) {
	result.a = s->getScript_type();
	result.b = s->getContents();
	return result;
      }
  }

  result.b = std::string("NOT FOUND NS ") + ns;
  result.a = scheme;
  return result;
}

void Rootcontainer::saveNs (std::string ns, std::string content)
{
  for(auto iter = getScripts()->begin(); iter != getScripts()->end(); iter++) {
    Script *scr = toScript(iter->second);
    if(scr->getNs() == ns) {
      scr->setContents(content);
      break;
    }
  }
}

bool Rootcontainer::containsNs (std::string ns) {
  for(auto iter = getScripts()->begin(); iter != getScripts()->end(); iter++) {
    Script *scr = toScript(iter->second);
    if(scr->getNs() == ns) return true;
  }
  
  return false;
}

Propertierbase* Rootcontainer::fetchRegister(std::string type, std::string id) {
  if(type == "AnimatedSprite") return (*getAnimatedsprites()).at(id);
  else if(type == "Layer") return (*getLayers()).at(id);
  else if (type == "Map") return (*getMaps()).at(id);
  else if (type == "Script") return (*getScripts()).at(id);
  else if (type == "Sprite") return (*getSprites()).at(id);
  else if (type == "Tile") return (*getTiles()).at(id);
  else if (type == "Tileset") return (*getTilesets()).at(id);

  qDebug() << "Didn't recognize type " << type.c_str();

  return nullptr;
}

bool Rootcontainer::typeHasId(std::string type, std::string id)
{
  if(type == "AnimatedSprite") return getAnimatedsprites()->find(id) != getAnimatedsprites()->end();
  else if(type == "Layer") return getLayers()->find(id) != getLayers()->end();
  else if (type == "Map") return getMaps()->find(id) != getMaps()->end();
  else if (type == "Script") return getScripts()->find(id) != getScripts()->end();
  else if (type == "Sprite") return getSprites()->find(id) != getSprites()->end();
  else if (type == "Tile") return getTiles()->find(id) != getTiles()->end();
  else if (type == "Tileset") return getTilesets()->find(id) != getTilesets()->end();  
}
  
int Rootcontainer::registrySize() {
  return getAnimatedsprites()->size() + 
    getLayers()->size() + 
    getMaps()->size() + 
    getScripts()->size() + 
    getSprites()->size() + 
    getTiles()->size() + 
    getTilesets()->size();
}

template <typename T>
void addToList(std::vector<Propertierbase*> &vec, std::map<std::string, T*>* m) {
  for(auto a = m->begin(); a != m->end(); a++) {
    vec.push_back(a->second);
  }
}

template <typename T>
void addToList(std::vector<Propertierbase*> *vec, std::map<std::string, T*>* m) {
  for(auto a = m->begin(); a != m->end(); a++) {
    vec->push_back(a->second);
  }
}
		     
std::vector<Propertierbase*> Rootcontainer::registryToList() {
  std::vector<Propertierbase*> vec;

  addToList<animatedsprite>(vec, getAnimatedsprites());
  addToList<Layer>(vec, getLayers());
  addToList<Map>(vec, getMaps());
  addToList<Script>(vec, getScripts());
  addToList<Sprite>(vec, getSprites());
  addToList<Tile>(vec, getTiles());
  addToList<Tileset>(vec, getTilesets());

  return vec;
}

#define addData(X) { addToList(vec, X); return vec; }

std::vector<Propertierbase*>* Rootcontainer::registryOf (std::string type) {

  if(type == "std::string" ||
     type == "std::vector<std::vector<Tile>>" ||
     type == "std::vector<Layer>" ||
     type == "float") return nullptr;

  std::vector<Propertierbase*>* vec = new std::vector<Propertierbase*>;

  if(type == "AnimatedSprite") {
    addToList(vec, getAnimatedsprites());
    return vec;
  }
  else if(type == "Layer")  addData(getLayers())
  else if (type == "Map") addData(getMaps())
  else if (type == "Script") addData(getScripts())
  else if (type == "Sprite") addData(getSprites())
  else if (type == "Tile") addData(getTiles())
  else if (type == "Tileset") addData(getTilesets())

  qDebug() << "Didn't recognize type " << type.c_str() << " at " << __FILE__ << ": " << __LINE__;
  throw "";
  return nullptr;
}

template <>
animatedsprite* Rootcontainer::nth(int i) {
  auto it = getAnimatedsprites()->begin();
  for(int ii=0; ii < i; ii++); it++;
  return (*it).second;
}

template <>
Map* Rootcontainer::nth(int i) {
  auto it = getMaps()->begin();
  for(int ii=0; ii < i; ii++); it++;
  return (*it).second;
}
