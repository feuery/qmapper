#ifndef map_inclguard
#define map_inclguard
//// generated at #<date-time 2017-09-09 22:10:21.957 {1002E7ACC3}>
#include<cstring>
#include<propertierbase.h>
#include<root.h>
#include<layer.h>
#include<vector>
#include<string>
class root;
class layer;
class map: public Propertierbase 
 { 
public: 
std::vector<layer*>*  layers = nullptr;
std::string getName();

void setName(std::string val);


std::string name_field = "Map 1";
virtual void parent(root* p) = 0;
virtual int width(void) = 0;
virtual int height(void) = 0;
virtual root* parent(void) = 0;
public: 
map();
const char* type_name(const char* propertyname); 
virtual const char** names() { return r; }
int property_count = 1;
virtual void set(const char* propertyname, std::string value) {
if(strcmp(propertyname, "name") == 0) name_field = value;
}

virtual std::string get(const char* propertyname, bool *success, std::string* type_helper) {
if(strcmp(propertyname, "name") == 0) {

*success = true;
return name_field;
}*success = false; std::string a; return a;
}
const char* r[1];
virtual const char* type_identifier() { return "map"; }};
#endif