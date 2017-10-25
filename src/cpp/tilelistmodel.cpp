// http://web.archive.org/web/20150224011819/http://qt-project.org/doc/qt-4.8/itemviews-editabletreemodel-treemodel-cpp.html

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
    int rows = r->registry->size();
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
  
  const char *type = base->type_identifier().get().c_str();
  
  if(strcmp(type, "Map") == 0) {
    Map *m = static_cast<Map*>(base);
    return createIndex(row, column, m->layers->at(row));
  }
  else if(strcmp(type, "root") == 0) {
    auto index = Root->indexOf(row);
    return createIndex(row, column, Root->registry->at(index));
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
  const char *type = base->type_identifier().get().c_str();
  int row = index.row();

  std::string helper;
  bool succ = false;
  std::string name = base->get(flyweight<std::string>(std::string("name")), &succ, helper);

  if(succ) { return QString(name.c_str()); }
  
  if(strcmp(type, "root") == 0) {
    return QString("Root");
  }
  else if(strcmp(type, "Map") == 0) {
    Map *m = static_cast<Map*>(base);
    return QString(m->getName().c_str());
  }
  else if(strcmp(type, "Layer") == 0) {
    Layer *l = static_cast<Layer*>(base);
    return QString(l->getName().c_str());
  }
  else if(strcmp(type, "Script") == 0) {
    Script *s = static_cast<Script*>(base);
    return QString(s->getName().c_str());
  }
  else if(strcmp(type, "Tileset") == 0) {
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

  const char* type = obj->type_identifier().get().c_str();

  if(strcmp(type, "Map") == 0) {
    Map *m = static_cast<Map*>(obj);
    return createIndex(Root->rowOf(m->getId()), 0, Root);
  }
  else if(strcmp(type, "Layer") == 0) {
    Layer *l = static_cast<Layer*>(obj);
    Map *p = l->parent();

    if(!p) {
      qDebug() << "Layer " << l << " has invalid parent " << p;
      return QModelIndex();
    }
    // qDebug() << "Trying to get parent of " << p;
    // qDebug() << ", with id " << p->getId().get().c_str();
    int row = Root->rowOf(p->getId());
    return createIndex(row, 0, p);
  }
  else if (strcmp(type, "Script") == 0) {
    Script *s = static_cast<Script*>(obj);
    return createIndex(Root->rowOf(s->getId()), 0, Root);
  }
  else if(strcmp(type, "Texture") == 0 ) {
    Texture *t = static_cast<Texture*>(obj);
    return createIndex(Root->rowOf(t->getId()), 0, Root);
  }
  else if(strcmp(type, "Tileset") == 0 ) {
    Tileset *t = static_cast<Tileset*>(obj);
    return createIndex(Root->rowOf(t->getId()), 0, Root);
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
  auto mapId = Root->indexOf(map_row);
  int count = toMap(Root->registry->at(mapId))->layers->size();
  beginInsertRows(parent_index, count, count+1);
}

void Tilelistmodel::begin()
{
  auto root = QModelIndex();
  int count = Root->registry->size();
  beginInsertRows(root, count, count + 1);
}
