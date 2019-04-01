#ifndef holderr
#define holderr

#include <guile_fn.h>

class holder {
public:
  cl_object getValue() const;
  void setValue(cl_object v);
  holder(bool engine_doc = false);
private:
  bool engine_doc;
};

#endif
