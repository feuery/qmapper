#ifndef guile_fnnn
#define guile_fnnn

#include <ecl/ecl.h>
#include <string>

std::string type_name(cl_object record);
cl_object get(cl_object record, const char *prop);
cl_object set(cl_object record, const char *prop, cl_object value);

cl_object lisp(const std::string & call);
std::string ecl_string_to_string(cl_object echar);
cl_object makefn(const char *fn);
cl_object find_by_id(const char *id);

void setCallLogs(bool val);

void run_swank();

#define DEFUN(name,fun,args)						\
  cl_def_c_function(c_string_to_object((std::string("exported-")+name).c_str()), \
		    (cl_objectfn_fixed)fun,		\
		    args);				\
  lisp((std::string("(setf qmapper.export:")+name+" #'exported-"+name+")").c_str());

cl_object l_lambda(cl_objectfn_fixed fun, int args);

#define LAMBDA(fun,args) l_lambda((cl_objectfn_fixed) fun, args)


#endif
