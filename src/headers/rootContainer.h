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
  
  flyweight<std::string> indexOf (int row) override;
  int rowOf(flyweight<std::string> id) override;

  either<scriptTypes, std::string> findNs (std::string ns) override;
  void saveNs (std::string ns, std::string content) override;

  bool containsNs (std::string ns);

private:

};

#endif //ROOTCONTAINER_H
