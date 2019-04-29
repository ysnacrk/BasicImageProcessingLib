#include "histogram.h"
#include "ui_histogram.h"

histogram::histogram(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::histogram)
{
  ui->setupUi(this);
}

histogram::~histogram()
{
  delete ui;
}

void histogram::drawHistogram(int *array){

  QCPBars *myBars = new QCPBars(ui->widget2->xAxis, ui->widget2->yAxis);
  myBars->setName("Bars Series 1");
  QVector<double> keyData;
  QVector<double> valueData;
  for(int i = 0 ; i< 256; i++) keyData.append(i);
  for(int i = 0 ; i< 256; i++) valueData.append(array[i]);

  myBars->setData(keyData, valueData);
  ui->widget2->rescaleAxes();
  ui->widget2->replot();
}
