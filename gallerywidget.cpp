#include "gallerywidget.h"
#include "ui_gallerywidget.h"

GalleryWidget::GalleryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GalleryWidget)
{
    ui->setupUi(this);
}

GalleryWidget::~GalleryWidget()
{
    delete ui;
}
