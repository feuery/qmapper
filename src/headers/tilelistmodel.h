#ifndef TILELISTMODEL_H
#define TILELISTMODEL_H

#include <QAbstractListModel>
#include <cstdio>
#include <unistd.h>
// #include <editorController.h>

#include <ecl/ecl.h>
#include <holder.h>
#include <unordered_map>


// Layers are parents/root nodes if needed
class Tilelistmodel: public QAbstractItemModel
{
  Q_OBJECT
public:

  Tilelistmodel(holder Root);

  // qabstractitemmodel-data
  QVariant data(const QModelIndex &index, int role) const override;
  /* Qt::ItemFlags flags(const QModelIndex &index) const override; */
  QVariant headerData(int section, Qt::Orientation orientation,
		      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
		    const QModelIndex &parent = QModelIndex()) const override;
  // QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent) const override;
  QModelIndex parent(const QModelIndex &child) const;

  bool hasChildren(const QModelIndex &parent) const override;

  void begin(int r);
  void begin();
  void beginMap(int map_row);
  void end();

  static Tilelistmodel instance;

private:

  holder Root;
};

cl_object deref_index(const QModelIndex &parent);
int indexOf(cl_object vec, cl_object element);


extern int int_cache;
int getInt();

#endif //TILELISTMODEL_H
