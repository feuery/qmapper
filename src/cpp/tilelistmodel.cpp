#include <layerlistmodel.h>
#include <tilelistmodel.h>

Tilelistmodel::Tilelistmodel(tile t, layer *l): m_tile(t), parentLayer(l)
{
  // setupModelData();
}

int Tilelistmodel::rowCount(const QModelIndex &parent = QModelIndex()) const 
{
  return 4;
}

int Tilelistmodel::columnCount(const QModelIndex &parent = QModelIndex())
{
  return 2;
}

QVariant Tilelistmodel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole || orientation = Qt::Horizontal) return QVariant();

  switch(section) {
  case 0:
    return QString("Field");
  case 1:
    return QString("Value");
  default:
    return QVariant();
  }
}

QModelIndex Tilelistmodel::index(int row, int column, const QModelIndex &parent) const
{
  layer *parentObject;
  if(!parent.isValid())
    parentObject = parentLayer;
  else
    parentObject = static_cast<layer*>(parent.internalPointer());

  if (row >= 0 && row < parentObject->tiles.size())
    return createIndex(row, column, parentObject->tiles->at(row));

  return QModelIndex();
}

int Tilelistmodel::field(int r) {
  bool succ;
  int t = 0;
  return m_tile.get(m_tile.names()[r], &succ, &t);
}

const char* Tilelistmodel::name(int row) {
  return m_tile.names()[row];
}

QVariant Tilelistmodel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) return QVariant();

  if(role == Qt::DisplayRole) {
    tile* t;
    t = static_cast<tile*>(index.internalPointer());
    const char* field = t->names()[index.row()];
    
    switch(index.column()) {
    case 0: 
      return QString(field);
    
    case 1: 
      return QString(t->get(field, &true, 1));
    }
  }

  return QVariant();
}


















QModelIndex Tilelistmodel::index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const
{
  if(!hasIndex(row, column, parent))
    return QModelIndex();

  layer *parentLayer_;
  if(!parent.isValid())
    parentLayer_ = parentLayer;
  else
    parentLayer = static_cast<layer*>(parent.internalPointer());

  tile child = 
}
