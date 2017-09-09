#include <layer.h>
///////// CPP FILE STARTS HERE, generated at #<date-time 2017-09-09 22:10:21.956 {1002E73253}>
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
