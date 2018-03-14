#include <tilelistmodel.h>
#include<rootContainer.h>
#include <script.h>
#include <QDebug>

Rootcontainer::Rootcontainer(): root()
{
  // TODO replace with a vector<unique_pointer<map*>> or smthng which doesn't leak like a sieve
  setRegistry(new std::map<std::string, std::map<std::string, Propertierbase*>>);
  // (*registry)["Layer"];
  (*(getRegistry()))["Map"];
  (*(getRegistry()))["Root"];
  (*(getRegistry()))["Script"];
  (*(getRegistry()))["Tile"];
  (*(getRegistry()))["Tileset"];
  (*(getRegistry()))["Sprite"];
  (*(getRegistry()))["AnimatedSprite"];
}

Rootcontainer::~Rootcontainer()
{
  delete getRegistry();
}

either<scriptTypes, std::string> Rootcontainer::findNs(std::string ns)
{
  std::vector<Script*> scripts;

  either<scriptTypes, std::string> result;

  for(auto iter = getRegistry()->at("Script").begin(); iter != getRegistry()->at("Script").end(); iter++) {
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
  for(auto iter = getRegistry()->at("Script").begin(); iter != getRegistry()->at("Script").end(); iter++) {
    Script *scr = toScript(iter->second);
    if(scr->getNs() == ns) {
      scr->setContents(content);
      break;
    }
  }
}

bool Rootcontainer::containsNs (std::string ns) {
  for(auto iter = getRegistry()->at("Script").begin(); iter != getRegistry()->at("Script").end(); iter++) {
    Script *scr = toScript(iter->second);
    if(scr->getNs() == ns) return true;
  }
  
  return false;
}

Propertierbase* Rootcontainer::fetchRegister(std::string type, std::string id) {
  return getRegistry()->at(type).at(id);
}
  
void Rootcontainer::doRegister(std::string type, std::string id, Propertierbase *o) {
  (*(getRegistry()))[type][id] = o;
}

int Rootcontainer::registrySize() {
  int sum = 0;
  for(auto i = getRegistry()->begin(); i != getRegistry()->end(); i++) {
    sum += i->second.size();
  }
  return sum;
}
		     
int Rootcontainer::typeRegistrySize(std::string type_name) {
  return getRegistry()->at(type_name).size();
}

void Rootcontainer::erase(std::string type, std::string id) {
  getRegistry()->at(type).erase(id);
}

void Rootcontainer::erase(std::string type, int id) {
  int counter = 0;
  for (auto i = getRegistry()->at(type).begin();
       i != getRegistry()->at(type).end() && counter < id;
       i++, counter++) {
    if(counter == id) {
      getRegistry()->at(type).erase(i);
      return;
    }
  }
}

std::vector<Propertierbase*> Rootcontainer::registryToList() {
  std::vector<Propertierbase*> v;

  return std::accumulate(getRegistry()->begin(),
			 getRegistry()->end(),
			 v,
			 [](std::vector<Propertierbase*>& v, auto subreg_i) {
			   std::map<std::string, Propertierbase*> &subreg = subreg_i.second;
			   for(auto i: subreg) {
			     v.push_back(i.second);
			   }
			   return v;});
}

std::vector<Propertierbase*> Rootcontainer::registryToList(std::vector<std::string> filterTypes) {
  std::vector<Propertierbase*> v;

  for(auto i = getRegistry()->begin(); i != getRegistry()->end(); i++) {
    if(std::find(filterTypes.begin(), filterTypes.end(), i->first) != filterTypes.end()) continue;

    for(auto i2: i->second)
      v.push_back(i2.second);
  }
  return v;
}

std::vector<Propertierbase*> Rootcontainer::registryOf (std::string type) {

  if(type == "std::string" ||
     type == "std::vector<std::vector<Tile>>" ||
     type == "std::vector<Layer>" ||
     type == "float") return {};
  
  std::vector<Propertierbase*> r;

  for(auto i: getRegistry()->at(type)){
    r.push_back(i.second);
  }
  return r;
}
