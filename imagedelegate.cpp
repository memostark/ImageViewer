#include "imagedelegate.h"

#include <QPainter>
#include <QImageReader>

ImageDelegate::ImageDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void ImageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    QPixmap pixmap = index.model()->data(index, Qt::DecorationRole).value<QPixmap>();
    painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

    painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return index.model()->data(index, Qt::SizeHintRole).value<QSize>();
}
