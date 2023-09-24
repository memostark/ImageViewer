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

    QString path = index.model()->data(index, Qt::DisplayRole).toString();

    QPixmap original(path);
    int columnWidth = option.rect.width();
    QSize size;
    if (original.width() > columnWidth) {
        float ratio = static_cast<float>(original.width()) / static_cast<float>(original.height());
        int newHeight = columnWidth / ratio;
        size = QSize(columnWidth, newHeight);
    } else {
        size = QSize(original.width(), original.height());
    }

    QPixmap pixmap = original.scaled(size);
    painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

    painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString path = index.model()->data(index, Qt::DisplayRole).toString();
    QImageReader reader(path);
    QSize original = reader.size();

    float ratio = static_cast<float>(original.width()) / static_cast<float>(original.height());;
    int newHeight = option.rect.width() / ratio;
    return QSize(option.rect.width(), newHeight);
}
