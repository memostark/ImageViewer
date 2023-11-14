#include "gallerywidget.h"
#include "imagedelegate.h"
#include "qimagereader.h"
#include "qlistview.h"
#include <QResizeEvent>
#include "qsettings.h"
#include "ui_gallerywidget.h"

#include <QScrollBar>

#include <filesystem>
namespace fs = std::filesystem;

GalleryWidget::GalleryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GalleryWidget),
    mListModel(new ImageModel())
{
    ui->setupUi(this);

    ui->listView->setModel(mListModel);
    ui->listView->setItemDelegate(new ImageDelegate(this));

    ui->listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listView ->verticalScrollBar()->setSingleStep(25);

}

void GalleryWidget::resizeEvent(QResizeEvent *event) {
    QListView* listView = ui->listView;
    realListWidth = listView->width() - listView->verticalScrollBar()->width();

    QSettings settings;
    QString viewType = settings.value("selected_view_type", "").toString();
    if(viewType.isEmpty()) viewType = "List view";
    setLayoutType(viewType);

    qDebug() << "List width:" << listView->width();
    qDebug() << "Scrollbar width:" << listView->verticalScrollBar()->width();
}

GalleryWidget::~GalleryWidget()
{
    delete ui;
}

void GalleryWidget::updateList(const QString& folderPath) {
    qDebug() << "Update gallery list called" << folderPath;
    std::vector<Image*> list;
    // load the urls and update the listmodel
    for (const auto & entry : fs::directory_iterator(folderPath.toStdString())) {
        if (!entry.is_directory()) {
            auto path = QString::fromStdString(entry.path().string());
            QImageReader reader(path);

            if(!reader.format().isEmpty()){
                qDebug() << path;
                QImageReader reader(path);
                QSize original = reader.size();
                list.push_back(new Image(entry.path().string(), original.width(), original.height()));
            }
        }
    }

    mListModel->setImageList(list);
}

void GalleryWidget::setLayoutType(const QString& layoutType) {
    QListView* listView = ui->listView;

    if (layoutType == "Grid view") {
        listView->setFlow(QListView::LeftToRight);
        listView->setViewMode(QListView::IconMode);
        int size = realListWidth / 3 - 1;
        listView->setGridSize(QSize(size, size));
        listView->setWrapping(true);
    } else if (layoutType == "Collage view") {
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
        listView->setFlow(QListView::LeftToRight);
        listView->setWrapping(true);
    } else {
        listView->setFlow(QListView::TopToBottom);
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
        ui->listView->setWrapping(false);
    }
}
