#include <map.h>
///////// CPP FILE STARTS HERE, generated at #<date-time 2017-07-18 18:46:32.148 {1002ECA1A3}>
std::string map::getName() {
 return name_field; 
};
void map::setName(std::string val) {
 name_field = val; 
}
map::map(){
r[0] = "name";
}

const char* map::type_name(const char* propertyname) {
if (strcmp(propertyname, "name") == 0) return "std::string";
return "";
}