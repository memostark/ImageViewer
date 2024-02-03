#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include "imagemodel.h"
#include "qlabel.h"
#include <QMovie>
#include <QWidget>
#include <QStringListModel>
#include <QtConcurrent>

namespace Ui {
class GalleryWidget;
}

class GalleryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GalleryWidget(QWidget *parent = nullptr);
    ~GalleryWidget();

public slots:
    void updateList(const QString& folderPath);
    void setLayoutType(const QString& layoutType);

private:
    Ui::GalleryWidget *ui;
    QLabel* loadingIcon;
    QMovie* loadingMovie;
    ImageModel* mListModel;
    std::vector<Image*> rawList;

    int realListWidth;

    void resizeEvent(QResizeEvent *event) override;

    void readImagesData(const QString& folderPath);
    QFuture<void> calculateListSize(std::vector<Image*>& list, int columnWidth);
    QFuture<void> calculateCollageSizes(std::vector<Image*>& list);
    Image* updateSize (Image* image);
    float getAspectRatio(QSize& size);
    void normalizeHeights(const std::vector<Image *>& images, int count, int startIndex, float height);
};

#endif // GALLERYWIDGET_H
