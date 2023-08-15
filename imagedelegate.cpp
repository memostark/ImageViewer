#include "imagedelegate.h"

#include <QPainter>

const int height = 50;

ImageDelegate::ImageDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{

}

void ImageDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    QPixmap original("://res/placeholder-image.png");
    int columnWidth = option.rect.width();
    QSize size;
    if (original.width() > columnWidth) {
        float ratio = original.width() / original.height();
        int newHeight = columnWidth / ratio;
        size = QSize(columnWidth, newHeight);
    } else {
        size = QSize(original.width(), original.height());
    }

    QPixmap pixmap = original.scaled(size);
    painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);


    QRect textRect(option.rect);
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    painter->drawText(textRect, Qt::AlignLeft, text);

    painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QPixmap original("://res/placeholder-image.png");
    float ratio = original.width() / original.height();
    int newHeight = option.rect.width() / ratio;
    return QSize(option.rect.width(), newHeight);
}
