#ifndef ROOTCONTAINER_H
#define ROOTCONTAINER_H

#include<root.h>
#include<tilelistmodel.h>
#include <QStandardItemModel>

class Rootcontainer: public root{
public:

  Rootcontainer();
  ~Rootcontainer();
  
  flyweight<std::string> indexOf (int row) override;
  int rowOf(flyweight<std::string> id) override;
  

private:

};

#endif //ROOTCONTAINER_H
