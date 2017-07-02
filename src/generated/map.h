#ifndef map_inclguard
#define map_inclguard
//// generated at #<date-time 2017-07-02 19:06:35.016 {100360B3E3}>
#include<cstring>
#include<propertierbase.h>
#include<layer.h>
#include<vector>
#include<string>
class map: public Propertierbase 
 { 
public: 
std::vector<std::vector<layer>>*  layers = nullptr;
std::string  name = "Map 1";
public: 
map();
const char* type_name(const char* propertyname); 
virtual const char** names() { return r; }
int property_count = 0;
const char* r[0];
};
#endif