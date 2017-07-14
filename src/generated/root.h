#ifndef root_inclguard
#define root_inclguard
//// generated at #<date-time 2017-07-14 20:26:24.835 {1002EC9DA3}>
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