#ifndef holderr
#define holderr

#include <guile_fn.h>

class holder {
public:
  cl_object getValue() const;
  void setValue(cl_object v);
  holder();
private:
  cl_object setter = ECL_NIL;
};

#endif
