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
    QSize size = calculateSize(option.rect.width(), original.size());

    QPixmap pixmap = original.scaled(size);
    painter->drawPixmap(option.rect.x(), option.rect.y(), pixmap);

    painter->restore();
}

QSize ImageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString path = index.model()->data(index, Qt::DisplayRole).toString();
    QImageReader reader(path);
    QSize original = reader.size();

    return calculateSize(option.rect.width(), original);
}

QSize ImageDelegate::calculateSize(int columnWidth, QSize imageSize) const {
    QSize size;
    if (imageSize.width() > columnWidth) {
        float ratio = static_cast<float>(imageSize.width()) / static_cast<float>(imageSize.height());;
        int newHeight = columnWidth / ratio;
        size = QSize(columnWidth, newHeight);
    } else {
        size = QSize(imageSize.width(), imageSize.height());
    }

    return size;
}
