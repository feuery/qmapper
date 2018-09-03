#include <holder.h>

cl_object holder::getValue() const {
  return lisp("*document*");
}

void holder::setValue(cl_object v) {
  if(setter == ECL_NIL)
    setter = makefn("set-doc");
  cl_funcall(2, setter, v);
}

holder::holder() { }
