#include <tilelistmodel.h>
#include<rootContainer.h>
#include <script.h>
#include <QDebug>

Rootcontainer::Rootcontainer(): root()
{
  // TODO replace with a vector<unique_pointer<map*>> or smthng which doesn't leak like a sieve
  registry = new std::map<std::string, std::map<std::string, Propertierbase*>>;
  (*registry)["Layer"];
  (*registry)["Map"];
  (*registry)["Root"];
  (*registry)["Script"];
  (*registry)["Texture"];
  (*registry)["Tile"];
  (*registry)["Tileset"];
}

Rootcontainer::~Rootcontainer()
{
  delete registry;
}


// flyweight<std::string> Rootcontainer::indexOf (int row)
// {
//   auto it = registry->begin();
//   for(int i =0; i<row; i++)
//     it ++;
//   return it->first;
// }

int Rootcontainer::rowOf(flyweight<std::string> id)
{
  auto it = registry->find(id);
  if(it == registry->end()) return -1;
 
  return std::distance(registry->begin(), it);
}

either<scriptTypes, std::string> Rootcontainer::findNs(std::string ns)
{
  std::vector<Script*> scripts;

  either<scriptTypes, std::string> result;

  for(auto iter = registry->at("Script").begin(); iter != registry->at("Script").end(); iter++) {
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
  for(auto iter = registry->at("Script").begin(); iter != registry->at("Script").end(); iter++) {
    Script *scr = toScript(iter->second);
    if(scr->getNs() == ns) {
      scr->setContents(content);
      break;
    }
  }
}

bool Rootcontainer::containsNs (std::string ns) {
  for(auto iter = registry->at("Script").begin(); iter != registry->at("Script").end(); iter++) {
    Script *scr = toScript(iter->second);
    if(scr->getNs() == ns) return true;
  }
  
  return false;
}

Propertierbase* Rootcontainer::fetchRegister(std::string type, flyweight<std::string> id) {
  return registry->at(type).at(id.get());
}
  
void Rootcontainer::doRegister(std::string type, flyweight<std::string> id, Propertierbase *o) {
  (*registry)[type][id.get()] = o;
}

int Rootcontainer::registrySize() {
  int sum = 0;
  for(auto i = registry->begin(); i != registry->end(); i++) {
    sum += i->second.size();
  }
  return sum;
}
		     
int Rootcontainer::typeRegistrySize(std::string type_name) {
  return registry->at(type_name).size();
}

void Rootcontainer::erase(std::string type, std::string id) {
  registry->at(type).erase(id);
}

void Rootcontainer::erase(std::string type, int id) {
  int counter = 0;
  for (auto i = registry->at(type).begin();
       i != registry->at(type).end() && counter < id;
       i++, counter++) {
    if(counter == id) {
      registry->at(type).erase(i);
      return;
    }
  }
}

std::vector<Propertierbase*> Rootcontainer::registryToList() {
  std::vector<Propertierbase*> v;

  return std::accumulate(registry->begin(),
			 registry->end(),
			 v,
			 [](std::vector<Propertierbase*>& v, auto subreg_i) {
			   std::map<std::string, Propertierbase*> &subreg = subreg_i.second;
			   for(auto i: subreg) {
			     v.push_back(i.second);
			   }
			   return v;});
}

std::vector<Propertierbase*> Rootcontainer::registryOf (std::string type) {
  std::vector<Propertierbase*> r;

  for(auto i: registry->at(type)){
    r.push_back(i.second);
  }
  return r;
}
