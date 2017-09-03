#ifndef ROOTCONTAINER_H
#define ROOTCONTAINER_H

#include<root.h>
#include<tilelistmodel.h>
#include <QStandardItemModel>

class Rootcontainer: public root{
public:

  Rootcontainer();
  ~Rootcontainer();  

private:

};

QStandardItemModel* dump_to_model(root *R);

#endif //ROOTCONTAINER_H
