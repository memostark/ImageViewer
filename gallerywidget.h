#ifndef GALLERYWIDGET_H
#define GALLERYWIDGET_H

#include <QWidget>

namespace Ui {
class GalleryWidget;
}

class GalleryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GalleryWidget(QWidget *parent = nullptr);
    ~GalleryWidget();

private:
    Ui::GalleryWidget *ui;
};

#endif // GALLERYWIDGET_H
