#ifndef map_inclguard
#define map_inclguard
//// generated at #<date-time 2017-07-02 15:21:43.062 {10042D23C3}>
#include<cstring>
#include<propertierbase.h>
#include<layer.h>
#include<vector>
class map: public Propertierbase 
 { 
public: 
std::vector<std::vector<layer>>*  layers = nullptr;
public: 
map();
const char* type_name(const char* propertyname); 
virtual const char** names() { return r; }
int property_count = 0;
const char* r[0];
};
#endif