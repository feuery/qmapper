#include <QDebug>
#include <renderer.h>
#include <QHBoxLayout>
#include <QLabel>
#include <mainwindow.h>
 
MainWindow::MainWindow() : QDialog()
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  qDebug() << "JEEJEE";
  Renderer* r = new Renderer;
  qDebug() << "JEEJEE";
  layout->addWidget(new QLabel("Heiterve", this));
  layout->addWidget(r);
  
  // delete this->layout();
  setLayout(layout);
}
