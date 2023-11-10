#ifndef IMAGEDELEGATE_H
#define IMAGEDELEGATE_H

#include <QStyledItemDelegate>



class ImageDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ImageDelegate(QObject *parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QSize calculateSize(int columnWidth, QSize imageSize) const;
};

#endif // IMAGEDELEGATE_H
