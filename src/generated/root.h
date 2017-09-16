#ifndef roote
#define roote

#include<propertierbase.h>
#include<map.h>
#include<vector>
#include<string>
#include<cstring>
class Map;
class root: public Propertierbase {
 public: std::vector<Map*>* all_maps = nullptr;
public: root();

const char * r[0];
const char** names() { return r; }

virtual const char* type_identifier() { return "root"; }
virtual int property_count() { return 0; }
virtual const char* type_name(const char *propertyname) {
return "";
}

};
#endif