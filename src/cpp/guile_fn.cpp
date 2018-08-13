#include <QString>
#include <guile_fn.h>
#include <QApplication>

// TODO uudelleenkirjota scheme-filut lispillä

// guile_fn::guile_fn(const char *name)
// {
//   func = ecl_make_symbol(name, "CL-USER");
// }

std::string type_name(cl_object record) {
  cl_object type_of = ecl_make_symbol("type-of", "CL-USER");

  return ecl_string_to_string(cl_funcall(2, type_of, record));
}

cl_object get(cl_object record, const char *prop)
{
  static cl_object get = ecl_make_symbol("get-prop", "CL-USER");
  return cl_funcall(3, get, record, ecl_make_symbol(prop, "CL-USER"));
}

cl_object set(cl_object record, const char *prop, cl_object val)
{
  static cl_object set = ecl_make_symbol("set-prop", "CL-USER");

  return cl_funcall(4, set, record, ecl_make_symbol(prop, "CL-USER"), val);
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
  return lisp(std::string("(lambda (&rest rst) (format t \"Calling ") + fn + "~%\") (apply #'" + fn + " rst))");
}

static char start_swank[] =
  "./start-slime.lisp";

void run_swank() {
  QString path = "\""+QCoreApplication::applicationDirPath() +"/"+start_swank +"\"";
  cl_object cl_start_swank_path = c_string_to_object(path.toStdString().c_str());
  cl_object cl_load =  ecl_make_symbol("LOAD","CL");
  cl_funcall(2, cl_load, cl_start_swank_path);
}
