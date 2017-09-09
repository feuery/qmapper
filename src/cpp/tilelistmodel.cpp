// http://web.archive.org/web/20150224011819/http://qt-project.org/doc/qt-4.8/itemviews-editabletreemodel-treemodel-cpp.html

#include <QDebug>
#include <map.h>
#include <tilelistmodel.h>

Propertierbase* Tilelistmodel::getparent(const QModelIndex &parent) const {
  if(parent.isValid()) return static_cast<Propertierbase*>(parent.internalPointer());
  else return Root;
}

Tilelistmodel::Tilelistmodel(root *R): Root(R) { }

int Tilelistmodel::rowCount(const QModelIndex &qparent) const
{
  Propertierbase *parent = getparent(qparent);
  QModelIndex grandParent = qparent.parent();
  if(parent == nullptr) {
    throw "";
  }
  const char* type = parent->type_identifier();
  
  if(strcmp(type, "map") == 0) {
    map *Map = static_cast<map*>(parent);
    return Map->layers->size();
  }
  else if (strcmp(type, "root") == 0) {
    root *r = static_cast<root*>(parent);
    int rows = r->all_maps->size();
    return rows;
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(parent);
    return 0;
  }
  else {
    throw "";
  }
  return 0;
}

int Tilelistmodel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant Tilelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
    return QVariant();
  }

  return QString("Field");
}

QModelIndex Tilelistmodel::index(int row, int column, const QModelIndex &qparent) const
{
  Propertierbase *base = getparent(qparent);
  
  const char *type = base->type_identifier();
  
  if(strcmp(type, "map") == 0) {
    map *m = static_cast<map*>(base);
    return createIndex(row, column, m->layers->at(row));
  }
  else if(strcmp(type, "root") == 0) {
    return createIndex(row, column, Root->all_maps->at(row));
  }
  else {
    throw "";
  }
  
  return QModelIndex();
}

QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) return QVariant();
  if(role != Qt::DisplayRole) return QVariant();
  
  Propertierbase *base = getparent(index);
  const char *type = base->type_identifier();
  int row = index.row();

  if(strcmp(type, "root") == 0) {
    return QString("Root");
  }
  else if(strcmp(type, "map") == 0) {
    map *m = static_cast<map*>(base);
    return QString(m->getName().c_str());
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(base);
    return QString(l->getName().c_str());
  }

  return QVariant();
}

template<typename T>
int indexOf(std::vector<T>* vec, T element)
{
  auto it = std::find(vec->begin(), vec->end(), element);
  if (it != vec->end()) {
    return std::distance(vec->begin(), it);
  }

  return -1;
}

QModelIndex Tilelistmodel::parent(const QModelIndex &index) const
{
  if(!index.isValid()) return QModelIndex();
  
  Propertierbase *obj = getparent(index);
  if(obj == Root) return QModelIndex();

  const char* type = obj->type_identifier();

  if(strcmp(type, "map") == 0) {
    map *m = static_cast<map*>(obj);
    return createIndex(indexOf(Root->all_maps, m), 0, Root);
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(obj);
    map *p = l->parent();
    int row = indexOf(Root->all_maps, p);
    return createIndex(row, 0, p);
  }
  else {
    // printf("Got an unknown type at Tilelistmodel::parent(): %s\n", type);
    throw "";
  }
  return QModelIndex();
}

void Tilelistmodel::begin(int r)
{
  auto root = QModelIndex();
  beginInsertRows(root, r, r+1);
}
void Tilelistmodel::end()
{
  endInsertRows();
}


void Tilelistmodel::beginMap(int map_row)
{
  auto root = QModelIndex();
  QModelIndex parent_index = index(map_row, 0, root);
  int count = Root->all_maps->at(map_row)->layers->size();
  beginInsertRows(parent_index, count, count+1);
}
