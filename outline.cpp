#include "outline.h"
#include <QDebug>

OutlineItem::OutlineItem(const QList<QVariant> &data, OutlineItem *parent)
{
    parentItem=parent;
    itemData=data;
}

OutlineItem::~OutlineItem()
{
    qDeleteAll(childItems);
}

void OutlineItem::appendChild(OutlineItem *item)
{
    childItems.append(item);
}

OutlineItem *OutlineItem::child(int row)
{
    return childItems.value(row);
}

int OutlineItem::childCount() const
{
    return childItems.count();
}

int OutlineItem::columnCount() const
{
    return itemData.count();
}

QVariant OutlineItem::data(int column) const
{
    return itemData.value(column);
}

OutlineItem *OutlineItem::parent()
{
    return parentItem;
}

int OutlineItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<OutlineItem*>(this));

    return 0;
}

OutlineModel::OutlineModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Header type" << "Title";
    rootItem = new OutlineItem(rootData);
}

OutlineModel::~OutlineModel()
{
    delete rootItem;
}

int OutlineModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<OutlineItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant OutlineModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    OutlineItem *item = static_cast<OutlineItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags OutlineModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant OutlineModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex OutlineModel::index(int row, int column, const QModelIndex &parent)
const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    OutlineItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<OutlineItem*>(parent.internalPointer());

    OutlineItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex OutlineModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    OutlineItem *childItem = static_cast<OutlineItem*>(index.internalPointer());
    OutlineItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int OutlineModel::rowCount(const QModelIndex &parent) const
{
    OutlineItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<OutlineItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void OutlineModel::setupModelData(const QStringList &lines)
{
    QList<QVariant> data;
    data<<"abc"<<"def";
    //qDebug()<<rootItem->data(0);
    //new OutlineItem(data,rootItem);
    rootItem->appendChild(new OutlineItem(data,rootItem));
}
