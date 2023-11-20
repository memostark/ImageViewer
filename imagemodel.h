
#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include "image.h"
#include <QAbstractListModel>



class ImageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ImageModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setImageList(std::vector<Image*> newImages);
private:
    bool isIndexValid(const QModelIndex& index) const;

    std::vector<Image*> images;
};

#endif // IMAGEMODEL_H
