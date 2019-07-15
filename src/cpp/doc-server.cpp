#include <doc-server.h>
#include <editorController.h>
#include <guile_fn.h>

void document_server::start() {
  int c_port = 1000 + (rand() % 10000);
  cl_object run_server = makefn("qmapper.doc-server:run-tcp-server-threaded"),
    port = ecl_make_int(c_port);
  printf("started doc-server on port %d\n", c_port);
  cl_funcall(2, run_server, port);
}
