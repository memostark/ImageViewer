#include "imagedelegate.h"

#include <QPainter>

ImageDelegate::ImageDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void ImageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    QRect textRect(option.rect);
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->drawText(textRect, Qt::AlignLeft, text);

    painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& /*option*/, const QModelIndex& index) const
{
    return QSize(150, 40);
}
