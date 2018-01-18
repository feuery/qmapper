#include <QDebug>
#include <map.h>
#include <tilelistmodel.h>
#include <script.h>
#include <texture.h>
#include <tilesetContainer.h>

Propertierbase* Tilelistmodel::getparent(const QModelIndex &parent) const {
  if(parent.isValid()) return static_cast<Propertierbase*>(parent.internalPointer());
  else return Root;
}

Tilelistmodel::Tilelistmodel(root *R): Root(R) { }

#define type_err_print printf("Got an unknown type %s at line %d\n", type, __LINE__);

int Tilelistmodel::rowCount(const QModelIndex &qparent) const
{
  Propertierbase *parent = getparent(qparent);
  QModelIndex grandParent = qparent.parent();
  if(parent == nullptr) {
    throw "";
  }
  const char* type = parent->type_identifier().get().c_str();
  
  if(strcmp(type, "Map") == 0) {
    Map *m = static_cast<Map*>(parent);
    return m->layers->size();
  }
  else if (strcmp(type, "root") == 0) {
    root *r = static_cast<root*>(parent);
    int rows = r->registrySize();
    return rows;
  }
  else if(strcmp(type, "Layer") == 0 ||
	  strcmp(type, "Script") == 0 ||
	  strcmp(type, "Texture") == 0 ||
	  strcmp(type, "Tileset") == 0) {
    return 0;
  }
  else {
    type_err_print;
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
  
  auto type = base->type_identifier().get();
  
  if(type == "Map") {
    Map *m = static_cast<Map*>(base);
    Layer *l =  m->layers->at(row);
    return createIndex(row, column, l);
  }
  else if(type == "root") {
    auto reg = Root->registryToList({});
    return createIndex(row, column, reg.at(row));
  }
  else {
    printf("Unknown type %s\n", type);
    throw "";
  }
  
  return QModelIndex();
}

QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) return QVariant();
  if(role != Qt::DisplayRole) return QVariant();
  
  Propertierbase *base = getparent(index);
  std::string type = base->type_identifier().get();
  int row = index.row();

  std::string helper;
  bool succ = false;
  std::string name = base->get(flyweight<std::string>(std::string("name")), &succ, helper);

  if(succ) { return QString(name.c_str()); }
  
  if(type == "root") {
    return QString("Root");
  }
  else if(type == "Map") {
    Map *m = static_cast<Map*>(base);
    return QString(m->getName().c_str());
  }
  else if(type == "Layer") {
    Layer *l = static_cast<Layer*>(base);
    return QString(l->getName().c_str());
  }
  else if(type == "Script") {
    Script *s = static_cast<Script*>(base);
    return QString(s->getName().c_str());
  }
  else if(type == "Tileset") {
    Tileset *s = static_cast<Tileset*>(base);
    return QString(s->getName().c_str());
  }
  else {
    type_err_print;
    throw "";
  }

  return QVariant();
}

QModelIndex Tilelistmodel::parent(const QModelIndex &index) const
{
  if(!index.isValid()) return QModelIndex();
  
  Propertierbase *obj = getparent(index);
  if(obj == Root) return QModelIndex();

  std::string type = obj->type_identifier().get();

  auto reg = Root->registryToList({});
  
  if(type == "Map") {
    Map *m = static_cast<Map*>(obj);
    
    return createIndex(indexOf<Propertierbase*>(&reg, static_cast<Propertierbase*>(m)), 0, Root);
  }
  else if(type == "Layer") {
    Layer *l = static_cast<Layer*>(obj);
    Map *p = l->parent();

    if(!p) {
      qDebug() << "Layer " << l << " has invalid parent " << p;
      return QModelIndex();
    }
    int row = indexOf<Layer*>(p->layers,l);
    
    return createIndex(row, 0, p);
  }
  else if (type == "Script") {
    Script *s = static_cast<Script*>(obj);
    return createIndex(indexOf<Propertierbase*>(&reg, static_cast<Propertierbase*>(s)), 0, Root);
  }
  else if(type == "Texture" ) {
    Texture *t = static_cast<Texture*>(obj);
    return createIndex(indexOf<Propertierbase*>(&reg, static_cast<Propertierbase*>(t)), 0, Root);
  }
  else if(type == "Tileset" ) {
    Tileset *t = static_cast<Tileset*>(obj);
    return createIndex(indexOf<Propertierbase*>(&reg, static_cast<Propertierbase*>(t)), 0, Root);
  }
  else {
    printf("Got an unknown type at Tilelistmodel::parent(): %s\n", type);
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
  auto map = Root->registryToList({}).at(map_row);
  int count = toMap(map)->layers->size();
  beginInsertRows(parent_index, count, count+1);
}

void Tilelistmodel::begin()
{
  auto root = QModelIndex();
  int count = Root->registrySize();
  beginInsertRows(root, count, count + 1);
}
