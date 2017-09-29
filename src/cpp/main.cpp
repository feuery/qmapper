#include <QtDebug>
#include <QApplication>
#include <guile_thread.h>
#include <editorController.h>

#include "mainwindow.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  MainWindow window(argc, argv);  // = new MainWindow(&app);
  qRegisterMetaType<Propertierbase*>();
  window.setWindowState(window.windowState()|Qt::WindowMaximized);
  window.show();
  int r = app.exec();

  return r;
}
