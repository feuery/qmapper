#ifndef Mape
#define Mape

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<root.h>
#include<layer.h>
#include<vector>
#include<string>
#include<texture.h>
#include<resize-enums.h>
#include<cstring>
class root;
class Layer;
class Texture;
class Map: public Propertierbase {
 public: std::vector<Layer*>* layers = nullptr;
public: virtual void setName(std::string val);
virtual std::string getName();
std::string Name_field = "Map 1";
public: virtual void setText(Texture* val);
virtual Texture* getText();
Texture* Text_field = nullptr;
public: virtual void parent (root* p) = 0;
public: virtual int width () = 0;
public: virtual int height () = 0;
public: virtual void resize (int w, int h, verticalAnchor vAnchor, horizontalAnchor hAnchor) = 0;
public: virtual root* parent () = 0;virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("name") ) { Name_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, Texture* value) {
if(propertyname == std::string("text") ) { Text_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
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
virtual Texture* get(flyweight<std::string> propertyname, bool *success, Texture* type_helper) {
if(propertyname == std::string("text")) {
  *success = true;
  return Text_field;
} *success = false; Texture* invalid_data; return invalid_data;
}
public: Map();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Map"); }
virtual int property_count() { return 3; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("name")) return flyweight<std::string>("std::string");
if(propertyname == std::string("text")) return flyweight<std::string>("Texture");return flyweight<std::string>("");
}

};

Map* toMap(Propertierbase *b);
#endif