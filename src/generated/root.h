#ifndef root_inclguard
#define root_inclguard
//// generated at #<date-time 2017-07-14 19:54:25.444 {1002EC0E33}>
#include<cstring>
#include<propertierbase.h>
#include<map.h>
#include<vector>
#include<string>
class root: public Propertierbase 
 { 
public: 
std::vector<map*>*  all_maps = nullptr;
public: 
root();
const char* type_name(const char* propertyname); 
virtual const char** names() { return r; }
int property_count = 0;
const char* r[0];
virtual const char* type_identifier() { return "root"; }};
#endif