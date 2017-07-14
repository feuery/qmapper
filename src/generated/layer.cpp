#include <layer.h>
///////// CPP FILE STARTS HERE, generated at #<date-time 2017-07-14 20:26:24.834 {1002EBB363}>
std::string layer::getName() {
 return name_field; 
};
void layer::setName(std::string val) {
 name_field = val; 
}
layer::layer(){
r[0] = "name";
}

const char* layer::type_name(const char* propertyname) {
if (strcmp(propertyname, "name") == 0) return "std::string";
return "";
}
