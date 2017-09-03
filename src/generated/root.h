#ifndef root_inclguard
#define root_inclguard
//// generated at #<date-time 2017-07-19 19:03:23.135 {1002ECF0A3}>
#include<cstring>
#include<propertierbase.h>
#include<map.h>
#include<vector>
#include<string>
class map;
class root: public Propertierbase 
 { 
public: 
std::vector<map*>*  all_maps = nullptr;
    virtual bool valid() { return true;}
public: 
root();
const char* type_name(const char* propertyname); 
virtual const char** names() { return r; }
int property_count = 0;
const char* r[0];
virtual const char* type_identifier() { return "root"; }};
#endif
