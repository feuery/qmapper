#include <guile_thread.h>
#include <guile_fn.h>
#include <QFileInfo>
#include <QDebug>

// #include <libguile.h>
#include <editorController_guile.h>
#include <files.h>

void lisp_loader::load_lisp() {

  QString path = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath() + "/";

  cl_object load = makefn("load");
  cl_funcall(2, load, lisp(("(pathname \""+path+"loadable_qmapper.lisp\")").toStdString().c_str()));

  run_swank();
    
  const char *works = "\nEverything's loaded";
  puts(works);
  
  running = true;
  
}
