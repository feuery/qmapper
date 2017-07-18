#ifndef TILELISTMODEL_H
#define TILELISTMODEL_H

#include <QAbstractListModel>
#include <root.h>

// Layers are parents/root nodes if needed
class Tilelistmodel: public QAbstractItemModel
{
public:

  Tilelistmodel(root *Root);

  // qabstractitemmodel-data
  QVariant data(const QModelIndex &index, int role) const override;
  // Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const override;
  // QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent) const override;

  // TODO implement
  QModelIndex parent(const QModelIndex &child) const;


private:

  root *Root;

  Propertierbase* getparent(const QModelIndex &parent) const;
};

#endif //TILELISTMODEL_H