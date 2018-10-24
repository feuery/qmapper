#include <holder.h>

cl_object holder::getValue() const {
  return lisp("qmapper.root:*document*");
}

void holder::setValue(cl_object v) {
  if(setter == ECL_NIL)
    setter = makefn("qmapper.root:set-doc");
  cl_funcall(2, setter, v);
}

holder::holder() { }
