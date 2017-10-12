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

std::string Rootcontainer::findNs(std::string ns)
{
  std::vector<Script*> scripts;
  
  for(auto iter = registry->begin(); iter != registry->end(); iter++) {
    // qDebug() << "Is \"" << iter->second->type_identifier().get().c_str() << "\" = \"Script\"";
    // if (iter->second->type_identifier() == flyweight<std::string>(std::string("Script"))) {
    //   qDebug() << "Yes, ";
    //   qDebug() << "Is " << toScript(iter->second)->getNs().c_str() << " = " << ns.c_str();
    // }
    
    if(iter->second->type_identifier() == flyweight<std::string>(std::string("Script"))) {
      if(toScript(iter->second)->getNs() == ns) {
	// qDebug() << "Yes";
	return toScript(iter->second)->getContents();
      }
    } // else qDebug() << "No";
    // qDebug() << "==========================================================";
  }

  return std::string("NOT FOUND NS ") + ns;
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
