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

void run_swank();


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
