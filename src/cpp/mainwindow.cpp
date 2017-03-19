#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <mainwindow.h>
#include <QFileDialog>
 
MainWindow::MainWindow() : QDialog()
{
  QHBoxLayout *layout = new QHBoxLayout(this);
  QVBoxLayout *toolbox_container = new QVBoxLayout(this);
  qDebug() << "JEEJEE";
  r = new Renderer;
  qDebug() << "JEEJEE";

  QPushButton *btn_texture = new QPushButton("Set texture to all surfaces ", this);
  connect(btn_texture, &QPushButton::clicked, this, &MainWindow::setTexture_clicked);
  
  toolbox_container->addWidget(btn_texture);
  toolbox_container->addWidget(new QPushButton("Create new surface ", this));
  layout->addLayout(toolbox_container);
  layout->addWidget(r);
  
  // delete this->layout();
  setLayout(layout);
  
}

void MainWindow::setTexture_clicked(bool checked)
{
  QString texture_path = QFileDialog::getOpenFileName(NULL, "Open a texture file", "~", "Images (*.png *.jpeg *.jpg *.bmp");
  
  r->texturizeDrawingQueue(texture_path);
}
  
