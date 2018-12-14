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
  cl_object get = makefn("qmapper.std:get-prop"),
    prin = makefn("prin1"),
    format = makefn("format");
  std::string literal = std::string("\"") + prop + "\"";
  // printf("getting %s\n", prop);

  cl_object key = c_string_to_object(literal.c_str());
  // cl_funcall(2, prin, key);
  // puts("");
  puts("trying to GET() at guile_fn");
  // cl_funcall(4, format, ECL_T, c_string_to_object("\"record ~a~%\""), record);
  cl_object result = cl_funcall(3, get, record, key);
  puts("did GET() at guile_fn");

  // puts("Result:");
  // cl_funcall(2, prin, result);

  return result;  
}

cl_object set(cl_object record, const char *prop, cl_object val)
{
  static cl_object set = makefn("qmapper.std:set-prop");

  return cl_funcall(4, set, record, c_string_to_object(prop), val);
}

cl_object lisp(const std::string & call) {
 return cl_safe_eval(c_string_to_object(call.c_str()), Cnil, Cnil);
}

std::string ecl_string_to_string(cl_object echar) {
    std::string res("");
    int j = echar->string.dim; //get dimension   
    ecl_character* selv = echar->string.self; //get pointer   

    //do simple pointer addition
    for(int i=0;i<j;i++){
        res += (*(selv+i));
    }
    return res;
}

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
  cl_object find = makefn("qmapper.root:find-by-id");

  return cl_funcall(3, find, ec->document.getValue(), c_string_to_object((std::string("\"")+id+"\"").c_str()));
}
