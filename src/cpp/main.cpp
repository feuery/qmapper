#include <QtDebug>
#include <QApplication>
#include <ecl/ecl.h>
#include <guile_thread.h>
#include <editorController.h>
#include <guile_qt_keymapping.h>

#include "mainwindow.h"

int main(int argc, char** argv)
{
  srand (time(NULL));
  cl_boot(argc, argv);
  init_keypairs();
  QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
  QApplication app(argc, argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(4,3);
  format.setProfile(QSurfaceFormat::CoreProfile);

  QSurfaceFormat::setDefaultFormat(format);
  
  MainWindow window(argc, argv);  // = new MainWindow(&app);
  window.setWindowState(window.windowState()|Qt::WindowMaximized);
  window.show();
  int r = app.exec();

  return r;
}
