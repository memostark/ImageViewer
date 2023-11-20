
#include "imagemodel.h"
#include "qsize.h"


ImageModel::ImageModel(QObject *parent) :
    images{}
{

}

int ImageModel::rowCount(const QModelIndex &parent) const
{
    return images.size();
}

QVariant ImageModel::data(const QModelIndex &index, int role) const
{
    if (!isIndexValid(index)) {
        return QVariant();
    }

    const Image& picture = *images.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(picture.fileUrl());
        break;

    case Qt::SizeHintRole:
        return QSize(picture.width(), picture.height()) ;
        break;

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ImageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "filepathToDisplay";
    roles[Qt::SizeHintRole] = "size";
    return roles;
}

void ImageModel::setImageList(std::vector<Image*> newImages)
{
    beginResetModel();
    images = newImages;
    endResetModel();
}

bool ImageModel::isIndexValid(const QModelIndex& index) const
{
    if (index.row() < 0
        || index.row() >= rowCount()
        || !index.isValid()) {
        return false;
    }
    return true;
}
