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
    ui->listView->verticalScrollBar()->setSingleStep(25);
    ui->listView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical { width: 20px; }");
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    loadingIcon = new QLabel(this);
    loadingIcon->setVisible(false);
    loadingMovie = new QMovie(":/res/icons/loading-icon.gif");
    loadingIcon->setMovie(loadingMovie);
    loadingMovie->start();
}

void GalleryWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    QListView* listView = ui->listView;
    realListWidth = listView->contentsRect().width() - listView->verticalScrollBar()->sizeHint().width();

    qDebug() << "List width:" << listView->width();
    qDebug() << "Scrollbar width:" << listView->verticalScrollBar()->sizeHint().width();

    QSize loadingSize = loadingMovie->currentPixmap().size();
    int xPos = (this->width() - loadingSize.width()) / 2;
    int yPos = (this->height() - loadingSize.height()) / 2;
    loadingIcon->setGeometry(xPos, yPos, loadingSize.width(), loadingSize.height());
}

GalleryWidget::~GalleryWidget()
{
    delete ui;
}

void GalleryWidget::updateList(const QString& folderPath) {
    qDebug() << "Update gallery list called" << folderPath;
    loadingIcon->setVisible(true);
    rawList.clear();

    QFuture<void> readDataFuture = QtConcurrent::run(&GalleryWidget::readImagesData, this, folderPath);
    readDataFuture.then([this](){
        QSettings settings;
        QString viewType = settings.value("selected_view_type", "").toString();
        if(viewType.isEmpty()) viewType = "List view";
        setLayoutType(viewType);
    });
}

void GalleryWidget::setLayoutType(const QString& layoutType) {
    calculateSizesFuture.cancel();

    loadingIcon->setVisible(true);
    QListView* listView = ui->listView;

    std::vector<Image *> newList;
    for (auto image: rawList) {
        newList.push_back(new Image(*image));
    }
    mListModel->resetThumbnails();

    if (layoutType == "Grid view") {
        listView->setFlow(QListView::LeftToRight);
        listView->setViewMode(QListView::IconMode);
        int size = realListWidth / 3 - 1;
        listView->setGridSize(QSize(size, size));
        listView->setWrapping(true);
        calculateSizesFuture = calculateListSize(newList, size);
    } else if (layoutType == "Collage view") {
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
        listView->setFlow(QListView::LeftToRight);
        listView->setWrapping(true);
        calculateSizesFuture = calculateCollageSizes(newList);
    } else {
        listView->setFlow(QListView::TopToBottom);
        listView->setViewMode(QListView::ListMode);
        listView->setGridSize(QSize(-1, -1));
        ui->listView->setWrapping(false);
        calculateSizesFuture = calculateListSize(newList, realListWidth);
    }

    calculateSizesFuture.then([this, newList](){
        mListModel->setImageList(newList);
        loadingIcon->setVisible(false);
    });
}

void GalleryWidget::readImagesData(const QString &folderPath)
{
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
}

QFuture<void> GalleryWidget::calculateListSize(std::vector<Image*>& list, int columnWidth) {

    return QtConcurrent::run([this, list, columnWidth](QPromise<void>& promise){
        for (auto image: list){
            if(promise.isCanceled()) return;

            if (image->width() > columnWidth) {
                float ratio = static_cast<float>(image->width()) / static_cast<float>(image->height());;
                int newHeight = columnWidth / ratio;

                image->setWidth(columnWidth);
                image->setHeight(newHeight);
            }
            auto path = QString::fromStdString(image->fileUrl());
            mListModel->generateThumbnail(path, QSize(image->width(), image->height()));
        }
    });
}

float arMin = 2.0f;
float arMax = 3.0f;

QFuture<void> GalleryWidget::calculateCollageSizes(std::vector<Image*>& list) {

    return QtConcurrent::run([this, list](QPromise<void>& promise){
        float arSum = 0;
        float width = static_cast<float>(realListWidth);

        int rowCount = 0;

        // index where the current row starts
        int i = 0;

        for (auto image: list){
            if(promise.isCanceled()) return;

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
    });
}

float GalleryWidget::getAspectRatio(QSize& size)
{
    return static_cast<float>(size.width()) / static_cast<float>(size.height());;
}

void GalleryWidget::normalizeHeights(const std::vector<Image *>& images, int count, int startIndex, float height)
{
    auto updateImages = std::vector<Image *>(images.begin() + startIndex,
                                         images.begin() + startIndex + count);

    for (auto image: updateImages){
        QSize size = QSize(image->width(), image->height());
        int width = height * getAspectRatio(size);
        image->setWidth(width);
        image->setHeight(height);

        auto path = QString::fromStdString(image->fileUrl());
        mListModel->generateThumbnail(path, QSize(width, height));
    }
}
