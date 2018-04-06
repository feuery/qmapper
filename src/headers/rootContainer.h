#ifndef ROOTCONTAINER_H
#define ROOTCONTAINER_H

#include<root.h>
#include<tilelistmodel.h>
#include <QStandardItemModel>
#include <either.h>
#include <QDebug>

class Rootcontainer: public root{
public:

  Rootcontainer();
  ~Rootcontainer();

  either<scriptTypes, std::string> findNs (std::string ns) override;
  void saveNs (std::string ns, std::string content) override;

  bool containsNs (std::string ns);

  Propertierbase* fetchRegister (std::string Type, std::string id) override;
  bool typeHasId(std::string type, std::string id);
  template <typename T>
  std::vector<T*>typeRegistry(std::string type_name) {
    std::vector<T*> vec;
    auto reg = *registryOf(type_name);
    for(auto t: reg) {
      vec.push_back(static_cast<T*>(t));
    }

    return vec;
  }

  int registrySize();
  std::vector<Propertierbase*>* registryOf (std::string type) override;

  std::vector<Propertierbase*> registryToList() override;
  template<typename T>
  T* nth(int i);

private:

};

template <typename T>
T* Rootcontainer::nth(int i) {
  qDebug() << QString("Specialized ::nth wrong");
}

#endif //ROOTCONTAINER_H
