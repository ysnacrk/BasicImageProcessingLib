#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.xpm *.jpg)"));

    dialog.setViewMode(QFileDialog::Detail);

    QString fileName = QFileDialog::getOpenFileName(this , tr("Open Images"),"/home/yasin/Desktop/deneme.png" , tr("Image Files (*.png *.jpg *.bmp)"));
       if(!fileName.isEmpty()){
       QImage image(fileName);
       ui -> label_pic ->setPixmap(QPixmap::fromImage(image));
       ui->label_pic->resize(ui->label_pic->pixmap()->size());

    }

    bmp.readImage(fileName.toUtf8().constData());
    string path = (QDir::currentPath() + "gray").toUtf8().constData();
    bmp.saveGrayScale(path);
    QImage image2(QDir::currentPath() + "gray");
    ui -> label_pic_2 ->setPixmap(QPixmap::fromImage(image2));
    bmp.cropImage(0,0,16,16);
    ui->label_pic_2->resize(ui->label_pic->pixmap()->size());
}
