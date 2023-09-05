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

private:
    Ui::GalleryWidget *ui;
    QStringListModel* mListModel;
};

#endif // GALLERYWIDGET_H
