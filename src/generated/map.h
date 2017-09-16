#ifndef Mape
#define Mape

#include<propertierbase.h>
#include<root.h>
#include<layer.h>
#include<vector>
#include<string>
#include<cstring>
class root;
class Layer;
class Map: public Propertierbase {
 public: std::vector<Layer*>* layers = nullptr;
public: virtual void setName(std::string val);
virtual std::string getName();
std::string Name_field = "Map 1";
public: virtual void parent (root* p) = 0;
public: virtual int width (void) = 0;
public: virtual int height (void) = 0;
public: virtual root* parent (void) = 0;virtual void set(const char* propertyname, std::string value) {
if(strcmp(propertyname, "Name") == 0) { Name_field = value; return; } }virtual std::string get(const char* propertyname, bool *success, std::string type_helper) {
if(strcmp(propertyname, "Name") == 0) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
public: Map();

const char * r[1];
const char** names() { return r; }

virtual const char* type_identifier() { return "Map"; }
virtual int property_count() { return 1; }
virtual const char* type_name(const char *propertyname) {
if(strcmp(propertyname, "name") == 0) return "std::string";return "";
}

};
#endif