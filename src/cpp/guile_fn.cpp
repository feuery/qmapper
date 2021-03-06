#include <QString>
#include <guile_fn.h>
#include <QApplication>
#include <iostream>
#include <editorController.h>

#define FORCE_LOGS

// guile_fn::guile_fn(const char *name)
// {
//   func = ecl_make_symbol(name, "CL-USER");
// }

std::string type_name(cl_object record) {
  cl_object type_of = makefn("qmapper.std:q-type-of");

  std::string str = ecl_string_to_string(cl_funcall(2, type_of, record));
  std::cout << str << std::endl;
  return str;
}

cl_object get(cl_object record, const char *prop)
{
  cl_object get = makeSilentFn("qmapper.std:get-prop"),
    prin = makeSilentFn("prin1"),
    format = makeSilentFn("format");
  std::string literal = std::string("\"") + prop + "\"";
  
  cl_object key = c_string_to_object(literal.c_str());
  cl_object result = cl_funcall(3, get, record, key);
  
  return result;  
}

cl_object set(cl_object record, const char *prop, cl_object val)
{
  cl_object set = makefn("qmapper.std:set-prop");

  return cl_funcall(4, set, record, c_string_to_object(prop), val);
}

cl_object lisp(const std::string & call) {
 return cl_safe_eval(c_string_to_object(call.c_str()), Cnil, Cnil);
}

std::string ecl_string_to_string(cl_object echar) {
  switch (ecl_t_of(echar)) {
#ifdef ECL_UNICODE
  case t_string:
    if (!ecl_fits_in_base_string(echar)) {
      echar = cl_copy_seq(echar);
    } else {
      echar = si_copy_to_simple_base_string(echar);
    }
    break;
#endif
  case t_base_string:
    // OK
    break;
  default:
    // PRINT SOME ERROR
    return std::string(); // or raise an exception
  }

  std::string res("");
  int j = echar->base_string.dim; //get dimension   
  ecl_base_char* selv = echar->base_string.self; //get pointer   

  //do simple pointer addition
  for(int i=0;i<j;i++){
    res += (*(selv+i));
  }
  return res;
};

// std::string ecl_string_to_string(cl_object clechar) {
//   cl_object echar = clechar // cl_funcall(2, lisp("(lambda (txt) ;; (format t \"str's type is ~a~%\" (type-of txt))\n (coerce txt 'base-string))"), clechar)
//     ;

//   // printf("string is %s\n", clechar->string.self);

//   auto print_type = lisp("(lambda (a) (format t \"type of object to convert is ~a~%\" (type-of a)))");
//   cl_funcall(2, print_type, clechar);

//   int j = echar->string.dim;
//   char lol[j] = "";
//   sprintf(lol, "%s", clechar->string.self);
//   std::string res(lol);

//   // printf("Result: %s\n", res.c_str());
  
//   return res;
  
//   // std::string res("");
//   // int j = echar->string.dim; //get dimension   
//   // ecl_character* selv = echar->string.self; //get pointer   

//   // //do simple pointer addition
//   // for(int i=0;i<j;i++){
//   //   res += (*(selv+i));
//   // }
//   // return res;
// }

cl_object makeSilentFn(const char *fn) {
  return lisp(std::string("(lambda (&rest rst) (apply #'") + fn +" rst))");
}

cl_object makefn(const char *fn) {
  // (format t \"Params ~a~%\" rst)
  return lisp(std::string("(lambda (&rest rst) ;(if *call-logs* \n(format t \"Calling ") + fn + ", ~%\");)\n  (apply #'" + fn + " rst))");
}

static char start_swank[] =
  "./start-slime.lisp";

void run_swank() {
  QString path = "\""+QCoreApplication::applicationDirPath() +"/"+start_swank +"\"";
  cl_object cl_start_swank_path = c_string_to_object(path.toStdString().c_str());
  cl_object cl_load =  ecl_make_symbol("LOAD","CL");
  cl_funcall(2, cl_load, cl_start_swank_path);
}

void setCallLogs(bool val) {
  #ifdef FORCE_LOGS
  lisp(std::string("(setf *call-logs* ")+(val? "t":"nil")+")");
  #endif 
}

cl_object l_lambda(cl_objectfn_fixed fun, int args)
{
  auto fn_name = (std::string("fun")+std::to_string(rand())).c_str();

  cl_def_c_function(c_string_to_object(fn_name),
		    (cl_objectfn_fixed)fun,
		    args);
  lisp(std::string("(setf qmapper.export:lambdas (fset:with qmapper.export:lambdas '") + fn_name + " #'"+fn_name+ "))");
    
  

  return lisp(std::string("(lambda (&rest rst) (apply (fset:lookup qmapper.export:lambdas '")+fn_name+") rst))");
}

cl_object find_by_id(const char *id) {
  auto ec = editorController::instance;
  cl_object find = makefn("qmapper.root:find-by-id"),
    result = cl_funcall(3, find, ec->document.getValue(), c_string_to_object((std::string("\"")+id+"\"").c_str()));

  return result;
}
