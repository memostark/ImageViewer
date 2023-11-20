
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
    connect(this, &MainWindow::viewTypeChanged,
            ui->galleryWidget, &GalleryWidget::setLayoutType);

    QSettings settings;
    QString path = settings.value("main_path").toString();
    qDebug() << "Saved path:" << path;
    ui->pathLabel->setText(path);

    // Set the saved view preference
    QButtonGroup* group = ui->viewsButtonGroup;
    ui->listViewRadioBtn->setChecked(true); // default preference
    QString viewType = settings.value("selected_view_type", "").toString();
    if (!viewType.isEmpty()) {
        QList<QAbstractButton *> buttons = group->buttons();
        for (QAbstractButton * button : buttons)
            if (button->text() == viewType)
                button->setChecked(true);
    }

    connect(group, &QButtonGroup::buttonClicked,
            [this, group](QAbstractButton *button)
            {
                qDebug() << group->id(button);
                auto text = button->text();
                QSettings settings;
                settings.setValue("selected_view_type", text);
                emit viewTypeChanged(text);
            });
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
