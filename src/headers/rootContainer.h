#ifndef ROOTCONTAINER_H
#define ROOTCONTAINER_H

#include<root.h>
#include<tilelistmodel.h>
#include <QStandardItemModel>
#include <either.h>

class Rootcontainer: public root{
public:

  Rootcontainer();
  ~Rootcontainer();
  
  // flyweight<std::string> indexOf (int row) override;
  int rowOf(flyweight<std::string> id) override;

  either<scriptTypes, std::string> findNs (std::string ns) override;
  void saveNs (std::string ns, std::string content) override;

  bool containsNs (std::string ns);

  Propertierbase* fetchRegister (std::string Type, flyweight<std::string> id);
  void doRegister (std::string Type, flyweight<std::string> id, Propertierbase* object);

  int registrySize();
  int typeRegistrySize(std::string type_name);
  void erase(std::string type, std::string id) override;
  void erase(std::string type, int id) override;
  std::vector<Propertierbase*> registryOf (std::string type) override;

  std::vector<Propertierbase*> registryToList();
  template<typename T>
  T* nth(std::string type_name, int i);

private:

};

template <typename T>
T* Rootcontainer::nth(std::string type_name, int i) {
  auto r = registry->at(type_name);
  int index = 0;
  for(auto iterator: r) {
    index++;
    if(index == i)
      return static_cast<T*>(iterator.second);
  }
  return nullptr;
}

#endif //ROOTCONTAINER_H
