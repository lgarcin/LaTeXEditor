#ifndef OUTLINE_H
#define OUTLINE_H

#define QSCINTILLA_DLL

#include <QAbstractItemModel>

class OutlineItem
 {
 public:
     OutlineItem(const QList<QVariant> &data, OutlineItem *parent = 0);
     ~OutlineItem();

     void appendChild(OutlineItem *child);

     OutlineItem *child(int row);
     int childCount() const;
     int columnCount() const;
     QVariant data(int column) const;
     int row() const;
     OutlineItem *parent();

 private:
     QList<OutlineItem*> childItems;
     QList<QVariant> itemData;
     OutlineItem *parentItem;
 };

class OutlineModel : public QAbstractItemModel
 {
     Q_OBJECT

 public:
     OutlineModel(QObject *parent = 0);
     ~OutlineModel();

     QVariant data(const QModelIndex &index, int role) const;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
     QModelIndex index(int row, int column,
                       const QModelIndex &parent = QModelIndex()) const;
     QModelIndex parent(const QModelIndex &index) const;
     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     int columnCount(const QModelIndex &parent = QModelIndex()) const;
     void setupModelData(const QStringList &lines);

 private:
     OutlineItem *rootItem;
 };

#endif // OUTLINE_H
