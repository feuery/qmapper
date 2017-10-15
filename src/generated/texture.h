#ifndef Texturee
#define Texturee

#include<propertierbase.h>

#include<boost/flyweight.hpp>
using namespace boost::flyweights;
#include<string>
#include<QString>
#include<root.h>
#include<cstring>
;
class Texture: public Propertierbase {
 public: GLuint texture_handle = -1;
public: int texture_width = 0;
public: int texture_height = 0;
public: virtual void parent (root* p) = 0;
public: virtual root* parent () = 0;
public: virtual void setName(std::string val);
virtual std::string getName();
std::string Name_field = "New texture";virtual void set(flyweight<std::string> propertyname, flyweight<std::string> value) {
if(propertyname == std::string("Id") ) { Id_field = value; return; } }
virtual void set(flyweight<std::string> propertyname, std::string value) {
if(propertyname == std::string("name") ) { Name_field = value; return; } }virtual flyweight<std::string> get(flyweight<std::string> propertyname, bool *success, flyweight<std::string> type_helper) {
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
public: Texture();

std::vector<flyweight<std::string>> r;
std::vector<flyweight<std::string>> names() { return r; }

virtual flyweight<std::string> type_identifier() { return flyweight<std::string>("Texture"); }
virtual int property_count() { return 2; }
virtual flyweight<std::string> type_name(flyweight<std::string> propertyname) {
if(propertyname == std::string("Id")) return flyweight<std::string>("flyweight<std::string>");
if(propertyname == std::string("name")) return flyweight<std::string>("std::string");return flyweight<std::string>("");
}

};

Texture* toTexture(Propertierbase *b);
#endif