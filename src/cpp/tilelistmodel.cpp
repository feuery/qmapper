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
  Propertierbase *parent = getparent(qparent);

  const char* type = parent->type_identifier();

  if(strcmp(type, "map") == 0) {
    map *Map = static_cast<map*>(parent);
    return Map->layers->size();
  }
  else if (strcmp(type, "root") == 0) {
    root *r = static_cast<root*>(parent);
    return r->all_maps->size();
  }
  else {
    printf("Tilelistmodel got an unknown class %s\n", type);
    throw "";
  }
  return 0;
}

int Tilelistmodel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant Tilelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation == Qt::Horizontal) return QVariant();

  switch(section) {
  case 0:
    return QString("Field");
  case 1:
    return QString("Value");
  default:
    return QVariant();
  }
}

QModelIndex Tilelistmodel::index(int row, int column, const QModelIndex &qparent) const
{
  if (row >= 0 && row < rowCount(qparent) &&
      (column == 0 || column == 1)) {
    Propertierbase *base = getparent(qparent);
    const char *type = base->type_identifier();

    if(strcmp(type, "map") == 0) {
      map *m = static_cast<map*>(base);
      switch(column) {
      // case 0:
      // 	return createIndex(row, column, "Layer:");
      case 1:
	return createIndex(row, column, &m->layers->at(row));
      }
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
  if(!index.isValid()) return QVariant();

  if(role == Qt::DisplayRole) {
    Propertierbase *base = getparent(index);
    const char *type = base->type_identifier();
    if(strcmp(type, "root")) {
      root *r = static_cast<root*>(base);
      return QString(index.column() == 0? "Root": "");
    }
    else if(strcmp(type, "map") == 0) {
      map *r = static_cast<map*>(base);
      return QString(index.column() == 0? "Map": r->getName().c_str());
    }
    else if(strcmp(type, "layer") == 0) {
      layer *l = static_cast<layer*>(base);
      return QString(index.column() == 0? "Layer": l->getName().c_str());
    }
  }

  return QVariant();
}
