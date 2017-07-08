#ifndef LAYERLISTMODEL_H
#define LAYERLISTMODEL_H

#include <QAbstractListModel>
#include <layer.h>

class Layerlistmodel: QAbstractItemModel
{
public:

  Layerlistmodel(layer t);

  int childCount() const;
  int field(int row);
  const char* name(int row);

  // qabstractitemmodel-data
  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;


private:

  void setupModelData();

  layer m_layer;

};

#endif //LAYERLISTMODEL_H
