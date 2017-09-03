#ifndef SIMPLELISTMODEL_H
#define SIMPLELISTMODEL_H

#include <QAbstractListModel>
#include <root.h>

// Layers are parents/root nodes if needed
class Simplelistmodel: public QAbstractItemModel
{
  Q_OBJECT
public:

  Simplelistmodel(QObject *root, QObject *parent = 0);

  // qabstractitemmodel-data
  QVariant data(const QModelIndex &index, int role) const override;
  /* Qt::ItemFlags flags(const QModelIndex &index) const override; */
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role) const override;
  QModelIndex index(int row, int column,
		    const QModelIndex &parent) const override;
  // QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const;

  /* QModelIndex rootIndex(); */

private:

  QObject* m_root;

  Propertierbase* getparent(const QModelIndex &parent) const;
};

#endif //SIMPLELISTMODEL_H
