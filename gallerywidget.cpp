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
    mListModel(new ImageModel()),
    rawList()
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
    rawList.clear();
    // load the urls and update the listmodel
    for (const auto & entry : fs::directory_iterator(folderPath.toStdString())) {
        if (!entry.is_directory()) {
            auto path = QString::fromStdString(entry.path().string());
            QImageReader reader(path);

            if(!reader.format().isEmpty()){
                qDebug() << path;
                auto original = reader.size();
                rawList.push_back(new Image(entry.path().string(), original.width(), original.height()));
            }
        }
    }

    mListModel->setImageList(rawList);
}

void GalleryWidget::setLayoutType(const QString& layoutType) {
    QListView* listView = ui->listView;

    if (layoutType == "Grid view") {
        listView->setFlow(QListView::LeftToRight);
        listView->setViewMode(QListView::IconMode);
        int size = realListWidth / 3 - 1;
        listView->setGridSize(QSize(size, size));
        listView->setWrapping(true);
        mListModel->setImageList(rawList);
    } else if (layoutType == "Collage view") {
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
        listView->setFlow(QListView::LeftToRight);
        listView->setWrapping(true);
        std::vector<Image *> collageList;
        for (auto image: rawList) {
            collageList.push_back(new Image(*image));
        }
        calculateCollageSizes(collageList);
        mListModel->setImageList(collageList);
    } else {
        listView->setFlow(QListView::TopToBottom);
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
        ui->listView->setWrapping(false);
        mListModel->setImageList(rawList);
    }
}

float arMin = 2.0f;
float arMax = 3.0f;

void GalleryWidget::calculateCollageSizes(std::vector<Image*>& list) {
    float arSum = 0;
    float width = static_cast<float>(realListWidth);

    int rowCount = 0;

    // index where the current row starts
    int i = 0;
    for (auto image: list){

        QSize size = QSize(image->width(), image->height());
        float ratio = getAspectRatio(size);
        arSum += ratio;
        rowCount++;

        if (arSum >= arMin && arSum <= arMax) {
            normalizeHeights(list, rowCount, i, width / arSum);
            i += rowCount;
            arSum = 0;
            rowCount = 0;
        } else if(arSum > arMax) {
            arSum -= ratio;
            rowCount--;
            normalizeHeights(list, rowCount, i, width / arSum);
            i += rowCount;

            rowCount = 1;
            arSum = ratio;
        }
    }

    normalizeHeights(list, rowCount, list.size() - rowCount, width / arSum);
}

float GalleryWidget::getAspectRatio(QSize& size)
{
    return static_cast<float>(size.width()) / static_cast<float>(size.height());;
}

void GalleryWidget::normalizeHeights(std::vector<Image *>& images, int count, int startIndex, float height)
{
    auto updateImages = std::vector<Image *>(images.begin() + startIndex,
                                         images.begin() + startIndex + count);

    for (auto image: updateImages){
        QSize size = QSize(image->width(), image->height());
        int width = height * getAspectRatio(size);
        image->setWidth(width);
        image->setHeight(height);
    }
}
