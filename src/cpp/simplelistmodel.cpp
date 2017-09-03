#include <simplelistmodel.h>

Simplelistmodel::Simplelistmodel(QObject *root, QObject *parent )
{
  m_root = root;
}

QVariant Simplelistmodel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid()) {
    return QVariant();
  }

  if(role != Qt::DisplayRole) return QVariant();

  switch(index.column() )
    {
    case 0:
      return static_cast<QObject*>(index.internalPointer())->objectName();
    case 1:
      return static_cast<QObject*>(index.internalPointer())->metaObject()->className();
    }
  return QVariant();
}
  
// Qt::ItemFlags Simplelistmodel::flags(const QModelIndex &index) const
// {

// }

QVariant Simplelistmodel::headerData(int section, Qt::Orientation orientation,
				     int role) const
{
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return QVariant();

  switch(section)
    {
    case 0: return QString("Object");
    case 1: return QString("Class");
    }
  return QVariant();

}
QModelIndex Simplelistmodel::index(int row, int column,
				   const QModelIndex &parent = QModelIndex()) const
{
  QObject *parentObject;

  if(!parent.isValid())
    parentObject = m_root;
  else
    parentObject = static_cast<QObject*>(parent.internalPointer());

  if(row >= 0 && row < parentObject->children().count())
    return createIndex(row, column, parentObject->children().at(row));
  else return QModelIndex();
}

int Simplelistmodel::rowCount(const QModelIndex &parent = QModelIndex()) const
{
  QObject *parentObject;
  if(!parent.isValid())
    parentObject = m_root;
  else
    parentObject = static_cast<QObject*>(parent.internalPointer());

  return parentObject->children().count();
}

int Simplelistmodel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QModelIndex  Simplelistmodel::parent(const QModelIndex &index) const
{
  if(!index.isValid()) return QModelIndex();

  QObject *indexObject = static_cast<QObject*>(index.internalPointer());
  QObject *parentObject = indexObject->parent();

  if(parentObject == m_root) return QModelIndex();

  QObject *grandParentObject = parentObject->parent();
  return createIndex(grandParentObject->children().indexOf(parentObject),
		     0,
		     parentObject);
}

// QModelIndex  Simplelistmodel::rootIndex()
// {

// }

Propertierbase* Simplelistmodel::getparent(const QModelIndex &parent) const
{
}
