#include <QHBoxLayout>
#include <QLabel>
#include <mainwindow.h>
 
MainWindow::MainWindow() : QDialog()
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  QLabel *oikee;
  
  layout->addWidget(new QLabel("Heiterve", this));
  layout->addWidget(oikee = new QLabel("Heimoi", this));

  oikee->setAlignment(Qt::AlignRight);
  
  // delete this->layout();
  setLayout(layout);
}
