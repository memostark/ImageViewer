#include "gallerywidget.h"
#include "imagedelegate.h"
#include "qlistview.h"
#include <QResizeEvent>
#include "ui_gallerywidget.h"

#include <QScrollBar>

#include <filesystem>
namespace fs = std::filesystem;

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

    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView ->verticalScrollBar()->setSingleStep(25);

}

void GalleryWidget::resizeEvent(QResizeEvent *event) {
    QListView* listView = ui->listView;
    realListWidth = listView->width() - listView->verticalScrollBar()->width();

    qDebug() << "List width:" << listView->width();
    qDebug() << "Scrollbar width:" << listView->verticalScrollBar()->width();
}

GalleryWidget::~GalleryWidget()
{
    delete ui;
}

void GalleryWidget::updateList(const QString& folderPath) {
    qDebug() << "Update gallery list called" << folderPath;
    QStringList list;
    // load the urls and update the listmodel
    for (const auto & entry : fs::directory_iterator(folderPath.toStdString())) {
        qDebug() << entry.path().string();
        list << QString::fromStdString(entry.path().string());
    }

    mListModel->setStringList(list);
}

void GalleryWidget::setLayoutType(const QString& layoutType) {
    QListView* listView = ui->listView;

    if (layoutType == "Grid view") {
        listView->setFlow(QListView::LeftToRight);
        listView->setViewMode(QListView::IconMode);
        int size = realListWidth / 3 - 1;
        listView->setGridSize(QSize(size, size));
    } else {
        listView->setFlow(QListView::TopToBottom);
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
    }
}
