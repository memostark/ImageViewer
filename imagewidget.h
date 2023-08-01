#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget();

private:
    Ui::ImageWidget *ui;
};

#endif // IMAGEWIDGET_H
