#include "gallerywidget.h"
#include "imagedelegate.h"
#include "ui_gallerywidget.h"

GalleryWidget::GalleryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GalleryWidget),
    mListModel(new QStringListModel())
{
    ui->setupUi(this);

    QStringList list;
    list << "Test item a" << "Test item b" << "Test item c";
    mListModel->setStringList(list);

    ui->listView->setModel(mListModel);
    ui->listView->setItemDelegate(new ImageDelegate(this));
}

GalleryWidget::~GalleryWidget()
{
    delete ui;
}

void GalleryWidget::updateList(const QString& folderPath) {
    qDebug() << "Update gallery list called" << folderPath;
}
