#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include "imagemodel.h"
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
    ImageModel* mListModel;
    std::vector<Image*> rawList;

    int realListWidth;

    void resizeEvent(QResizeEvent *event) override;

    void createList(const QString& folderPath);
    void calculateListSize(std::vector<Image*>& list, int columnWidth);
    void calculateCollageSizes(std::vector<Image*>& list);
    Image* updateSize (Image* image);
    float getAspectRatio(QSize& size);
    void normalizeHeights(std::vector<Image *>& images, int count, int startIndex, float height);
};

#endif // GALLERYWIDGET_H
