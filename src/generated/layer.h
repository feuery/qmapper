#ifndef Layere
#define Layere

#include<propertierbase.h>
#include<tile.h>
#include<string>
#include<vector>
#include<map.h>
#include<cstring>
class Map;
class Tile;
class Layer: public Propertierbase {
 public: std::vector<std::vector<Tile>>* tiles = nullptr;
public: virtual void setName(std::string val);
virtual std::string getName();
std::string Name_field = "";
public: virtual int getWidth (void) = 0;
public: virtual int getHeight (void) = 0;
public: virtual void set_parent (Map* p) = 0;
public: virtual Map* parent (void) = 0;
protected: int w = 0;
protected: int h = 0;
protected: Map* p = nullptr;virtual void set(const char* propertyname, std::string value) {
if(strcmp(propertyname, "name") == 0) { Name_field = value; return; } }virtual std::string get(const char* propertyname, bool *success, std::string type_helper) {
if(strcmp(propertyname, "name") == 0) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
public: Layer();

const char * r[1];
const char** names() { return r; }

virtual const char* type_identifier() { return "Layer"; }
virtual int property_count() { return 1; }
virtual const char* type_name(const char *propertyname) {
if(strcmp(propertyname, "name") == 0) return "std::string";return "";
}

};
#endif