#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include <QWidget>
#include <QStringListModel>

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
    QStringListModel* mListModel;

    int realListWidth;

    void resizeEvent(QResizeEvent *event);
};

#endif // GALLERYWIDGET_H
