#include <guile_thread.h>
#include <guile_fn.h>
#include <QDebug>

// #include <libguile.h>
#include <editorController_guile.h>
#include <files.h>

#include <loader.h>

void lisp_loader::load_lisp() {
  // let's load quicklisp from ~/quicklisp/setup.lisp - TODO make configurable
  lisp(std::string("(progn (format t \"Starting qlisp initialization~%\") (let ((quicklisp-init (merge-pathnames \"quicklisp/setup.lisp\"                                        (user-homedir-pathname))))   (when (probe-file quicklisp-init)     (load quicklisp-init))))"));
  lisp("(ql:quickload :qmapper)");
    
  const char *works = "Everything's loaded";
  puts(works);
  
  running = true;
  
}
