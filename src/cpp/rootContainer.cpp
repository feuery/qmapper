#include <tilelistmodel.h>
#include<rootContainer.h>

Rootcontainer::Rootcontainer(): root()
{
  // TODO replace with a vector<unique_pointer<map*>> or smthng which doesn't leak like a sieve
  all_maps = new std::vector<map*>;
}

Rootcontainer::~Rootcontainer()
{
  delete all_maps;
}

QStandardItemModel* dump_to_model(root *root)
{
  QStandardItemModel *model = new QStandardItemModel;

  for(auto m = root->all_maps->begin(); m < root->all_maps->end(); m++) {
    QStandardItem *map_item = new QStandardItem(QString((*m)->getName().c_str()));
    for(auto layer = (*m)->layers->begin(); layer < (*m)->layers->end(); layer++) {
      QStandardItem *layer_item = new QStandardItem((*layer)->getName().c_str());
      map_item->appendRow(layer_item);
    }
    model->appendRow(map_item);
  }

  return model;
}
