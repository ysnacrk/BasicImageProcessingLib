#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QImage>
#include <QDir>

namespace Ui {
  class histogram;
}

class histogram : public QDialog
{
  Q_OBJECT

public:
  explicit histogram(QWidget *parent = nullptr);
  ~histogram();
  void drawHistogram(int * array);
private:
  Ui::histogram *ui;
};

#endif // HISTOGRAM_H
