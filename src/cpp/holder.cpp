#include <holder.h>

cl_object holder::getValue() const {
  return engine_doc? lisp("qmapper.root:*engine-document*"): lisp("qmapper.root:*document*");
}

void holder::setValue(cl_object v) {
  auto setter = engine_doc? lisp("(lambda (l) (setf qmapper.root:*engine-document* l))"): makefn("qmapper.root:set-doc");
  cl_funcall(2, setter, v);
}

holder::holder(bool engine_doc): engine_doc(engine_doc) { }
