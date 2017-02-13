#include <QtDebug>
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  MainWindow window;  // = new MainWindow(&app);
  window.show();
  return app.exec();
}
