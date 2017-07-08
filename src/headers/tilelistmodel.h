#ifndef TILELISTMODEL_H
#define TILELISTMODEL_H

#include <QAbstractListModel>
#include <tile.h>

// Layers are parents/root nodes if needed
class Tilelistmodel: QAbstractItemModel
{
public:

  Tilelistmodel(tile t, layer l);

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

  tile m_tile;
  layer *parentLayer;
  void setupModelData();
};

#endif //TILELISTMODEL_H
