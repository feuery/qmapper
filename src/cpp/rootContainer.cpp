#include <tilelistmodel.h>
#include<rootContainer.h>
#include <script.h>
#include <QDebug>

Rootcontainer::Rootcontainer(): root()
{
  // TODO replace with a vector<unique_pointer<map*>> or smthng which doesn't leak like a sieve
  registry = new std::map<flyweight<std::string>, Propertierbase*>;
}

Rootcontainer::~Rootcontainer()
{
  delete registry;
}


flyweight<std::string> Rootcontainer::indexOf (int row)
{
  auto it = registry->begin();
  for(int i =0; i<row; i++)
    it ++;
  return it->first;
}

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
  
  for(auto iter = registry->begin(); iter != registry->end(); iter++) {
    
    if(iter->second->type_identifier() == flyweight<std::string>(std::string("Script"))) {
      Script *s = toScript(iter->second);
      if(s->getNs() == ns) {
	result.a = s->getScript_type();
	result.b = s->getContents();
	return result;
      }
    } // else qDebug() << "No";
    // qDebug() << "==========================================================";
  }

  result.b = std::string("NOT FOUND NS ") + ns;
  result.a = scheme;
  return result;
}

void Rootcontainer::saveNs (std::string ns, std::string content)
{
  for(auto iter = registry->begin(); iter != registry->end(); iter++) {
    if(iter->second->type_identifier() == flyweight<std::string>(std::string("Script"))) {
      Script *scr = toScript(iter->second);
      if(scr->getNs() == ns) {
	scr->setContents(content);
	break;
      }
    }
  }
}

bool Rootcontainer::containsNs (std::string ns) {
  for(auto iter = registry->begin(); iter != registry->end(); iter++) {
    if(iter->second->type_identifier() == flyweight<std::string>(std::string("Script"))) {
      Script *scr = toScript(iter->second);
      if(scr->getNs() == ns) return true;
    }
  }
  
  return false;
}
