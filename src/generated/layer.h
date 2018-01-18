#ifndef Layere
#define Layere

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
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
public: virtual void setOpacity(unsigned char val);
virtual unsigned char getOpacity();
unsigned char Opacity_field = 0;
public: virtual void set_parent (Map* p) = 0;
public: virtual int getWidth () = 0;
public: virtual int getHeight () = 0;
public: virtual Map* parent () = 0;
protected: int w = 0;
protected: int h = 0;
protected: Map* p = nullptr;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, unsigned char value) {
if(propertyname == std::string("opacity") ) { Opacity_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
if(propertyname == std::string("Id")) {
  *success = true;
  return Id_field;
} *success = false; flyweight<std::string> invalid_data; return invalid_data;
}
virtual std::string get(flyweight<std::string> propertyname, bool *success, std::string type_helper) {
if(propertyname == std::string("name")) {
  *success = true;
  return Name_field;
} *success = false; std::string invalid_data; return invalid_data;
}
virtual unsigned char get(flyweight<std::string> propertyname, bool *success, unsigned char type_helper) {
if(propertyname == std::string("opacity")) {
  *success = true;
  return Opacity_field;
} *success = false; unsigned char invalid_data; return invalid_data;
}
public: Layer();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Layer"); }
virtual int property_count() { return 3; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("name")) return flyweight<std::string>("std::string");
if(propertyname == std::string("opacity")) return flyweight<std::string>("unsigned char");return flyweight<std::string>("");
}

};

Layer* toLayer(Propertierbase *b);
#endif