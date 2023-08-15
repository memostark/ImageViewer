
#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->folderBtn, &QPushButton::clicked,
            this, &MainWindow::setFolderPath);
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
}
