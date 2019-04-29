#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QImage>
#include <QDir>
#include "qcustomplot.h"
#include "histogram.h"

#include "BMP.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    BMP bmp;
    Ui::MainWindow *ui;
    histogram * hist;
};

#endif // MAINWINDOW_H

