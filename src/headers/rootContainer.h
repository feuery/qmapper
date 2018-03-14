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

  either<scriptTypes, std::string> findNs (std::string ns) override;
  void saveNs (std::string ns, std::string content) override;

  bool containsNs (std::string ns);

  Propertierbase* fetchRegister (std::string Type, std::string id);
  void doRegister (std::string Type, std::string id, Propertierbase* object);
  template <typename T>
  std::vector<T*>typeRegistry(std::string type_name) {
    std::vector<T*> vec;
    auto reg = registryOf(type_name);
    for(auto t: reg) {
      vec.push_back(static_cast<T*>(t));
    }

    return vec;
  }

  int registrySize();
  int typeRegistrySize(std::string type_name);
  void erase(std::string type, std::string id) override;
  void erase(std::string type, int id) override;
  std::vector<Propertierbase*> registryOf (std::string type) override;

  std::vector<Propertierbase*> registryToList();
  std::vector<Propertierbase*> registryToList(std::vector<std::string> filterTypes);
  template<typename T>
  T* nth(std::string type_name, int i);

private:

};

template <typename T>
T* Rootcontainer::nth(std::string type_name, int i) {
  auto r = getRegistry()->at(type_name);
  int index = 0;
  for(auto iterator: r) {
    index++;
    if(index == i)
      return static_cast<T*>(iterator.second);
  }
  return nullptr;
}

#endif //ROOTCONTAINER_H
