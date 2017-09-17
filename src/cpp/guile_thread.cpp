#include <guile_thread.h>
#include <QDebug>

#include <libguile.h>
#include <editorController_guile.h>

// TODO make a real c-guile-registration system
static void* register_functions(void* data) {
  scm_c_define_gsubr("add-map!", 3, 0, 0, reinterpret_cast<scm_t_subr>(add_map));
  scm_c_define_gsubr("add-layer!", 1, 0, 0, reinterpret_cast<scm_t_subr>(add_layer));
  scm_c_define_gsubr("delete-map!", 1, 0, 0, reinterpret_cast<scm_t_subr>(delete_map));
  scm_c_define_gsubr("delete-layer!", 2, 0, 0, reinterpret_cast<scm_t_subr>(delete_layer));
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
