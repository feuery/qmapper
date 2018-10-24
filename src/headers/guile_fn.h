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

void setCallLogs(bool val);

void run_swank();

#define DEFUN(name,fun,args)				\
  cl_def_c_function(c_string_to_object((std::string("exported-")+name).c_str()), \
		    (cl_objectfn_fixed)fun,		\
		    args);				\
  lisp((std::string("(setf qmapper.export:")+name+" #'exported-"+name+")").c_str());


// class guile_fn {
//   public:
//     guile_fn(const char *name);

//     template <typename... ArgTypes>
//     void apply(ArgTypes... args);
//     template <typename T, typename... ArgTypes>
//     void apply(T t, ArgTypes...args) {
//       puts("Lollo");
//       apply(args...);
//     }

//     template<> void apply() {}

//   private:

//     cl_object func;
    
//   }
  ;

#endif
