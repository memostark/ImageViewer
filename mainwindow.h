
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtimer.h"
#include "qevent.h"

#include <QMainWindow>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void folderChanged(const QString& newPath);
    void viewTypeChanged(const QString& viewType);

private slots:
    void setFolderPath();

private:
    Ui::MainWindow *ui;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // MAINWINDOW_H
