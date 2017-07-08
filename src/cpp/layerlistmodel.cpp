#include <layerlistmodel.h>

Layerlistmodel::Layerlistmodel(layer t): m_layer(t)
{
  setupModelData();
}

int Layerlistmodel::childCount() const
{
  return 4;
}

int Layerlistmodel::field(int r) {
  bool succ;
  int t = 0;
  return m_layer.get(m_layer.names()[r], &succ, &t);
}

const char* Layerlistmodel::name(int row) {
  return m_layer.names()[row];
}

QModelIndex Layerlistmodel::index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const
{
  if(!hasIndex(row, column, parent))
    return QModelIndex();

  
}
