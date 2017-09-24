#ifndef ROOTCONTAINER_H
#define ROOTCONTAINER_H

#include<root.h>
#include<tilelistmodel.h>
#include <QStandardItemModel>

class Rootcontainer: public root{
public:

  Rootcontainer();
  ~Rootcontainer();

  // std::vector<Map*> filterMaps() override;
  flyweight<std::string> indexOf (int row);
  int rowOf(flyweight<std::string> id);

private:

};

QStandardItemModel* dump_to_model(root *R);

#endif //ROOTCONTAINER_H
