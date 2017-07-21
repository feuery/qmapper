// http://web.archive.org/web/20150224011819/http://qt-project.org/doc/qt-4.8/itemviews-editabletreemodel-treemodel-cpp.html

#include <QDebug>
#include <map.h>
#include <tilelistmodel.h>

Propertierbase* Tilelistmodel::getparent(const QModelIndex &parent) const {
  return static_cast<Propertierbase*>(parent.isValid()?
				      parent.internalPointer(): Root);
}

Tilelistmodel::Tilelistmodel(root *R): Root(R)
{
  // setupModelData();
}

int Tilelistmodel::rowCount(const QModelIndex &qparent) const
{
    puts("Tilelistmodel::rowCount");
  Propertierbase *parent = getparent(qparent);

  const char* type = parent->type_identifier();
  
  printf("rowCount of type %s\n", type);

  if(strcmp(type, "map") == 0) {
    map *Map = static_cast<map*>(parent);
    auto rval = Map->layers->size();
    qDebug()<<"Returning " << rval;
    return rval;
  }
  else if (strcmp(type, "root") == 0) {
    root *r = static_cast<root*>(parent);
    auto rval = r->all_maps->size();
    qDebug()<<"Returning root's rowcount " << rval;
    return rval;
  }
  else {
    printf("Tilelistmodel got an unknown class %s\n", type);
    throw "";
  }
  auto rval = 0;
  qDebug()<<"Returning invalid rowcount " << rval;
  return rval;
}

int Tilelistmodel::columnCount(const QModelIndex &parent) const
{
  puts("columnCount");
  auto rval = 2;
  qDebug()<<"Returning " << rval;
  return rval;
}

QVariant Tilelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
    return QVariant();
  }

  switch(section) {
  case 0: {
    auto rval = QString("Field");
    // qDebug()<<"Returning " << rval;
    return rval;
  }
  case 1: {
    auto rval = QString("Value");
    // qDebug()<<"Returning " << rval;
    return rval;
  }
  default: {
    // puts("Returning default");
    return QVariant();
  }
  }
}

QModelIndex Tilelistmodel::rootIndex() {
  return createIndex(0,0,Root);
}

QModelIndex Tilelistmodel::index(int row, int column, const QModelIndex &qparent) const
{
  if (row >= 0 && row < rowCount(qparent) &&
      (column == 0 || column == 1)) {
    puts("Tilelistmodel::index");
    Propertierbase *base = getparent(qparent);
    const char *type = base->type_identifier();

    if(strcmp(type, "map") == 0) {
      map *m = static_cast<map*>(base);
      switch(column) {
      // case 0:
      // 	return createIndex(row, column, "Layer:");
      case 1:
	auto rval = createIndex(row, column, &m->layers->at(row));
	qDebug()<<"Returning layer " << rval;
	return rval;
      }
    }
    else if(strcmp(type, "root") == 0) {
      auto rval = createIndex(row, column, Root->all_maps->at(row));
      qDebug()<<"Returning map " << row << ", " << column << " - " << rval;
      if(column == 1) return rval;
    }
    // else if(strcmp(type, "layer") == 0) {
    //   layer *l = static_cast<layer*>(base);
    //   switch(column) {
    //   // case 0: return createIndex(row, column, "Name:");
    //   case 1:
    // 	return createIndex(row, column, l->getName().c_str());
    //   }
    // }
    else {
      printf("Tilelistmodel got an unknown class %s\n", type);
      throw "";
    }
  }

  return QModelIndex();
}


QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  puts("Tilelistmodel::data");
  if(!index.isValid()) return QVariant();

  if(role == Qt::DisplayRole) {
    Propertierbase *base = getparent(index);
    const char *type = base->type_identifier();
    if(strcmp(type, "root")) {
      // root *r = static_cast<root*>(base);
      auto rval = QString(index.column() == 0? "Root": "");
      qDebug()<<"Returning " << rval;
      return rval;
    }
    else if(strcmp(type, "map") == 0) {
      map *r = static_cast<map*>(base);
      auto rval = QString(index.column() == 0? "Map": r->getName().c_str());
      qDebug()<<"Returning " << rval;
      return rval;
    }
    else if(strcmp(type, "layer") == 0) {
      layer *l = static_cast<layer*>(base);
      auto rval = QString(index.column() == 0? "Layer": l->getName().c_str());
      qDebug()<<"Returning " << rval;
      return rval;
    }
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

QModelIndex Tilelistmodel::parent(const QModelIndex &child) const
{
  Propertierbase *obj = getparent(child);
  // if(obj == Root) return QModelIndex();

  puts("Tilelistmodel::parent()");

  const char* type = obj->type_identifier();
  if(strcmp(type, "map") == 0) {
    auto rval = createIndex(0, 0, Root);
    qDebug()<<"Returning " << rval;
    return rval;
  }
  else if(strcmp(type, "layer") == 0) {
    layer *l = static_cast<layer*>(obj);
    map *p = l->parent();
    int row = indexOf(Root->all_maps, p);
    auto rval = createIndex(row, 0, row);
    qDebug()<<"Returning " << rval;
    return rval;
  }
  else {
    printf("Got an unknown type at Tilelistmodel::parent(): %s\n", type);
    throw "";
  }
  return QModelIndex();
}
