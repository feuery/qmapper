#include <guile_thread.h>
#include <guile_fn.h>
#include <QFileInfo>
#include <QDebug>

// #include <libguile.h>
#include <editorController_guile.h>
#include <files.h>

void lisp_loader::load_lisp() {

  QString path = QFileInfo(QCoreApplication::applicationFilePath()).absolutePath() + "/";
  lisp("(require :asdf)");
  std::string command = ("(progn (format t \"Starting qlisp initialization~%\") (let ((quicklisp-init (merge-pathnames \"quicklisp/setup.lisp\"                                        (user-homedir-pathname)))) (format t \"does ~a exist?~%\" quicklisp-init) (if (probe-file quicklisp-init) (load quicklisp-init) (format t \"loading quicklisp failed. Does file ~a exist?~%\" quicklisp-init))) (require :ql) (require :asdf) (format t \"Quicklisp initialized!~%\") (push \"" + path + "\" asdf:*central-registry*))").toStdString();
  
  // let's load quicklisp from ~/quicklisp/setup.lisp - TODO make configurable
  lisp(command);
  lisp(("(format t \"Pushed " + path + " to central registry~%\")").toStdString());
  lisp("(ql:quickload :qmapper)");
  lisp("(require :qmapper)");
  lisp("(format t \"Quickloaded :qmapper?\")");

  run_swank();
    
  const char *works = "Everything's loaded";
  puts(works);
  
  running = true;
  
}
