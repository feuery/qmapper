// http://web.archive.org/web/20150224011819/http://qt-project.org/doc/qt-4.8/itemviews-editabletreemodel-treemodel-cpp.html

#include <QDebug>
#include <map.h>
#include <tilelistmodel.h>

Propertierbase* Tilelistmodel::getparent(const QModelIndex &parent) const {
  if(parent.isValid()) return static_cast<Propertierbase*>(parent.internalPointer());
  else return Root;
}

Tilelistmodel::Tilelistmodel(root *R): Root(R)
{
  // setupModelData();
}

int Tilelistmodel::rowCount(const QModelIndex &qparent) const
{
  Propertierbase *parent = getparent(qparent);
  QModelIndex grandParent = qparent.parent();
  if(parent == nullptr) {
    // puts("parent on invaliidi!");
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
    printf("%d riviä rootilla\n", rows);
    return rows;
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(parent);
    return 0;
  }
  else {
    // printf("Tilelistmodel got an unknown class %s\n", type);
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
    return createIndex(row, column, m);
  }
  else if(strcmp(type, "root") == 0) {
    return createIndex(row, column, Root);
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(base);
    return createIndex(row, column, l);
  }
  else {
    printf("Tilelistmodel got an unknown class %s\n", type);
    throw "";
  }
  
  return QModelIndex();
}

// Qt::ItemFlags Tilelistmodel::flags(const QModelIndex &index) const
// {
//   if(!index.isValid()) return Qt::ItemIsEnabled;

//   // Propertierbase *base = getparent(index);
//   // const char *type = base->type_identifier();

//   // if(strcmp(type, "layer") == 0) {
//   //   puts("Tyyppi on layer");
//   //   return Qt::ItemIsEnabled|Qt::ItemIsSelectable | Qt::ItemNeverHasChildren;
//   // }

//   // printf("Tyyppi ei ole layer vaan %s\n", type);
//   return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
// }

QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) return QVariant();
  if(!role != Qt::DisplayRole) return QVariant();
  
  Propertierbase *base = getparent(index);
  const char *type = base->type_identifier();
  int row = index.row();

  if(strcmp(type, "root") == 0) {
    puts("Palautetaan root-dataa");
    return QString("Root");
    
  }
  else if(strcmp(type, "map") == 0) {
    map *m = static_cast<map*>(base);
    printf("Palautetaan mappi %s\n", m->getName().c_str());
    return QString(m->getName().c_str());
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(base);
    return QString(l->getName().c_str());
  }
  else puts("Type didn't match");

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
    printf("Palautetaan mapin alaista riviä %d\n", row);
    return createIndex(row, 0, p);
  }
  else {
    // printf("Got an unknown type at Tilelistmodel::parent(): %s\n", type);
    throw "";
  }
  return QModelIndex();
}
