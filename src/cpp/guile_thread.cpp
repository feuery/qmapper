#include <guile_thread.h>
#include <QDebug>

#include <libguile.h>

// TODO make a real c-guile-registration system
static void* register_functions(void* data) {
  return NULL;
}

Guile_Thread::Guile_Thread(int argc, char** argv): QThread(), argc(argc), argv(argv) {
}

void Guile_Thread::run() {
  // while(running) {
  //   qDebug() << "Threaded stuff";
  //   sleep(2);
  // }
  scm_with_guile(&register_functions, NULL);
  scm_shell(argc, argv);
}
