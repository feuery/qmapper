#include <QString>
#include <guile_fn.h>
#include <QApplication>

// TODO uudelleenkirjota scheme-filut lispillä

// guile_fn::guile_fn(const char *name)
// {
//   func = ecl_make_symbol(name, "CL-USER");
// }

std::string type_name(cl_object record) {
  cl_object type_of = makefn("q-type-of");

  return ecl_string_to_string(cl_funcall(2, type_of, record));
}

cl_object get(cl_object record, const char *prop)
{
  cl_object get = makefn("get-prop"),
    prin = makefn("prin1")
    // format = makefn("format")
    ;
  std::string literal = std::string("\"") + prop + "\"";
  printf("getting %s\n", literal.c_str());

  cl_object key = c_string_to_object(literal.c_str());
  cl_funcall(2, prin, key);
  puts("");
  cl_object result = cl_funcall(3, get, record, key);

  puts("Result:");
  cl_funcall(2, prin, result);

  return result;  
}

cl_object set(cl_object record, const char *prop, cl_object val)
{
  static cl_object set = makefn("set-prop");

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

cl_object makefn(const char *fn) {
  // (format t \"Params ~a~%\" rst)
  return lisp(std::string("(lambda (&rest rst) (if *call-logs* (format t \"Calling ") + fn + ", ~%\"))  (apply #'" + fn + " rst))");
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
  lisp(std::string("(setf *call-logs* ")+(val? "t":"nil")+")");
}
