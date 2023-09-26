
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qsettings.h"

#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->folderBtn, &QPushButton::clicked,
            this, &MainWindow::setFolderPath);
    connect(this, &MainWindow::folderChanged,
            ui->galleryWidget, &GalleryWidget::updateList);

    QSettings settings;
    QString path = settings.value("main_path").toString();
    qDebug() << "Saved path:" << path;
    if (!path.isNull()) {
        ui->pathLabel->setText(path);
        emit folderChanged(path);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFolderPath(){

    QString directory = QFileDialog::getExistingDirectory(this,
                                                          "Pick a directory",
                                                          QDir::homePath(),
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!directory.isEmpty()){
        ui->pathLabel->setText(directory);
        emit folderChanged(directory);
        QSettings settings;
        settings.setValue("main_path", directory);
    }
}
