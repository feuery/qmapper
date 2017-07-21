#ifndef layer_inclguard
#define layer_inclguard
//// generated at #<date-time 2017-07-19 19:03:23.134 {1002EB6403}>
#include<cstring>
#include<propertierbase.h>
#include<tile.h>
#include<string>
#include<vector>
#include<map.h>
class map;
class layer: public Propertierbase 
 { 
public: 
std::vector<std::vector<tile>>*  tiles = nullptr;
std::string getName();

void setName(std::string val);


std::string name_field = "";
virtual int getwidth(void) = 0;
virtual int getheight(void) = 0;
virtual void set_parent(map* p) = 0;
virtual map* parent(void) = 0;
protected: 
int  w = 0;
int  h = 0;
map*  p = nullptr;
public: 
layer();
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
virtual const char* type_identifier() { return "layer"; }};
#endif